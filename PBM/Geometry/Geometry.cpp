﻿#include "Geometry.h"
#include <cmath>

using namespace std;

MeshData Geometry::CreateBox(float width, float height, float depth, const Vector & color)
{
	MeshData mesh;
	auto w2 = width / 2;
	auto h2 = height / 2;
	auto d2 = depth / 2;
	Vector vertex[24]{
		//上
		{-w2, h2, -d2, 1.0},{-w2, h2,  d2, 1.0},{w2, h2, d2, 1.0},{w2, h2, -d2, 1.0},
		//后
		{-w2, -h2, -d2, 1.0},{-w2, h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, -h2, -d2, 1.0},
		//下
		{-w2, -h2, -d2, 1.0},{w2, -h2, -d2, 1.0},{w2, -h2, d2, 1.0},{-w2, -h2, d2, 1.0},
		//前
		{-w2, -h2, d2, 1.0},{w2, -h2, d2, 1.0},{w2, h2, d2, 1.0},{-w2, h2, d2, 1.0},
		//左
		{-w2, -h2, -d2, 1.0},{-w2, -h2, d2, 1.0},{-w2, h2, d2, 1.0},{-w2, h2, -d2, 1.0},
		//右
		{w2, -h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, h2, d2, 1.0},{w2, -h2, d2, 1.0},
	};
	Vector normals[24]{
		//上
		{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
		//后
		{0, 0,-1,0},{0, 0, -1, 0},{0, 0, -1, 0},{0, 0, -1, 0},
		//下
		{0, -1, 0},{0, -1, 0},{0, -1, 0},{0, -1, 0},
		//前
		{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 0, 1},
		//左
		{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},
		//右
		{1, 0, 0},{1, 0, 0},{1, 0, 0},{1, 0, 0},
	};
	for (unsigned short i = 0; i < 24; ++i)
		mesh.Vertex.push_back({ vertex[i], color, normals[i] });
	unsigned short index[6] = { 0,1,2,2,3,0 };
	for (unsigned short i = 0; i < 6; ++i)
	{
		unsigned short base = i * 4;
		for (unsigned short j = 0; j < 6; ++j)
			mesh.TriangleIndices.push_back(index[j] + base);
	}
	return mesh;
}

MassSpringData Geometry::CreateBoxMS(float width, float height, float depth, float mass, float spring)
{
	MassSpringData data;
	auto w2 = width / 2;
	auto h2 = height / 2;
	auto d2 = depth / 2;
	Vector vertex[24]{
		//上
		{-w2, h2, -d2, 1.0},{-w2, h2,  d2, 1.0},{w2, h2, d2, 1.0},{w2, h2, -d2, 1.0},
		//后
		{-w2, -h2, -d2, 1.0},{-w2, h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, -h2, -d2, 1.0},
		//下
		{-w2, -h2, -d2, 1.0},{w2, -h2, -d2, 1.0},{w2, -h2, d2, 1.0},{-w2, -h2, d2, 1.0},
		//前
		{-w2, -h2, d2, 1.0},{w2, -h2, d2, 1.0},{w2, h2, d2, 1.0},{-w2, h2, d2, 1.0},
		//左
		{-w2, -h2, -d2, 1.0},{-w2, -h2, d2, 1.0},{-w2, h2, d2, 1.0},{-w2, h2, -d2, 1.0},
		//右
		{w2, -h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, h2, d2, 1.0},{w2, -h2, d2, 1.0},
	};
	size_t count = sizeof(vertex) / sizeof(Vector);
	for (size_t i = 0; i < count; ++i)
	{
		auto& v = vertex[i];
		auto curData = data.FindVertex(v);
		if (curData == nullptr)
			curData = data.AddVertex(v, mass);
		curData->m_MeshVertexIndex.push_back(i);
	}
	auto linkSquare = [vertex, spring, &data](size_t s) {
		size_t count = 4;
		for (size_t i = 0; i < count; ++i)
		{
			auto& v = vertex[i + s];
			auto& next = vertex[(i + s + 1) % count];
			data.AddHookLink(v, next, spring);
		}
	};
	// 上四方形
	linkSquare(0);
	// 下四方形
	linkSquare(8);
	//侧面四条边
	for (size_t i = 0; i < 4; ++i)
		data.AddHookLink(vertex[i], vertex[i + 8], spring);
	return data;
}

//levels：垂直切面个数，slices水平圆顶点个数
MeshData Geometry::CreateSphere(float radius, unsigned short levels, unsigned short slices, const Vector & color)
{
	MeshData meshData;
	unsigned short vertexCount = 2ui16 + levels * slices;
	//size_t indicesCount = 6 * levels * slices;
	//meshData.Vertex.resize(vertexCount);
	//meshData.TriangleIndices.resize(indicesCount);
	//存放顶点
	auto pushVertex = [&meshData, &color](const Vector& pos)
	{
		MeshVertex&& vertex = { pos, color, pos.Normalized() };
		meshData.Vertex.push_back(vertex);
	};
	pushVertex({ 0,radius,0,1 });
	float deltaY = 2 * radius / (levels + 1);
	float deltaRadian = 2 * XM_PI / slices;
	for (unsigned short i = 0; i < levels; ++i)
	{
		float y = radius - deltaY * (i + 1);
		float innerRadius = sqrt(radius * radius - y * y);
		for (unsigned short j = 0; j < slices; ++j)
		{
			float curRadian = deltaRadian * j;
			float x = innerRadius * cosf(curRadian);
			float z = innerRadius * sinf(curRadian);
			pushVertex({ x,y,z,1 });
		}
	}
	pushVertex({ 0,-radius,0,1 });
	//索引
	unsigned short base = 1;
	for (unsigned short i = 0; i < slices; ++i)
	{
		meshData.TriangleIndices.push_back(i + base);
		meshData.TriangleIndices.push_back(0);
		meshData.TriangleIndices.push_back((i + 1) % slices + base);
	}
	for (unsigned short i = 1; i < levels; ++i)
	{
		unsigned short start = 1 + i * slices;
		for (unsigned short j = 0; j < slices; ++j)
		{
			unsigned short next = (j + 1) % slices;
			meshData.TriangleIndices.push_back(start + j);
			meshData.TriangleIndices.push_back(start + j - slices);
			meshData.TriangleIndices.push_back(start + next);

			meshData.TriangleIndices.push_back(start + next);
			meshData.TriangleIndices.push_back(start + j - slices);
			meshData.TriangleIndices.push_back(start + next - slices);
		}
	}
	//已添加顶点数量=总数量-最下层数量
	base = vertexCount - (1 + slices);
	for (unsigned short i = 0; i < slices; ++i)
	{
		meshData.TriangleIndices.push_back(i + base);
		meshData.TriangleIndices.push_back((i + 1) % slices + base);
		meshData.TriangleIndices.push_back(vertexCount - 1);
	}
	return meshData;
}

MeshData Geometry::CreateGrid(float length, unsigned short count, const  Vector& color)
{
	MeshData data;
	float delta = length / count;
	float halfLength = length / 2;
	for (unsigned short i = 0; i <= count; ++i) {
		data.Vertex.push_back({ {i*delta - halfLength, 0, -halfLength, 1}, color });
		data.Vertex.push_back({ {i*delta - halfLength, 0, halfLength, 1}, color });
	}
	for (unsigned short i = 0; i <= count; ++i) {
		data.Vertex.push_back({ {-halfLength, 0, i*delta - halfLength, 1}, color });
		data.Vertex.push_back({ {halfLength, 0, i*delta - halfLength, 1}, color });
	}
	count = 2 * count;
	for (unsigned short i = 0; i <= count; ++i)
	{
		data.LineIndices.push_back(2 * i);
		data.LineIndices.push_back(2 * i + 1);
	}
	return data;
}

MeshData Geometry::CreateCircle(float radius, const Vector& center, unsigned short slices, const Vector & color)
{
	MeshData meshData;
	float deltaRadian = 2 * XM_PI / slices;
	for (unsigned short j = 0; j < slices; ++j)
	{
		float curRadian = deltaRadian * j;
		float x = radius * cosf(curRadian);
		float z = radius * sinf(curRadian);
		Vector pos = Vector({ x,0,z,1 }) + center.xyz();
		meshData.Vertex.push_back({ pos,  color, pos.Normalized() });
	}
	for (unsigned short j = 0; j < slices; ++j) {
		meshData.LineIndices.push_back(j);
		meshData.LineIndices.push_back((j + 1) % slices);
	}
	return meshData;
}
