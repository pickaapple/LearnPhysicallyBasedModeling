#include "Geometry.h"
#include <cmath>

using namespace std;

MeshData Geometry::CreateBox(float width, float height, float depth, const Vector & color)
{
	MeshData mesh;
	auto w2 = width / 2;
	auto h2 = height / 2;
	auto d2 = depth / 2;
	Vector vertex[24]{
		//��
		{-w2, h2, -d2, 1.0},{-w2, h2,  d2, 1.0},{w2, h2, d2, 1.0},{w2, h2, -d2, 1.0},
		//��
		{-w2, -h2, -d2, 1.0},{-w2, h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, -h2, -d2, 1.0},
		//��
		{-w2, -h2, -d2, 1.0},{w2, -h2, -d2, 1.0},{w2, -h2, d2, 1.0},{-w2, -h2, d2, 1.0},
		//ǰ
		{-w2, -h2, d2, 1.0},{w2, -h2, d2, 1.0},{w2, h2, d2, 1.0},{-w2, h2, d2, 1.0},
		//��
		{-w2, -h2, -d2, 1.0},{-w2, -h2, d2, 1.0},{-w2, h2, d2, 1.0},{-w2, h2, -d2, 1.0},
		//��
		{w2, -h2, -d2, 1.0},{w2, h2, -d2, 1.0},{w2, h2, d2, 1.0},{w2, -h2, d2, 1.0},
	};
	Vector normals[24]{
		//��
		{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},
		//��
		{0, 0,-1,0},{0, 0, -1, 0},{0, 0, -1, 0},{0, 0, -1, 0},
		//��
		{0, -1, 0},{0, -1, 0},{0, -1, 0},{0, -1, 0},
		//ǰ
		{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 0, 1},
		//��
		{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},
		//��
		{1, 0, 0},{1, 0, 0},{1, 0, 0},{1, 0, 0},
	};
	for (size_t i = 0; i < 24; ++i)
		mesh.Vertex.push_back({ vertex[i], color, normals[i] });
	unsigned short index[6] = { 0,1,2,2,3,0 };
	for (size_t i = 0; i < 6; ++i)
	{
		int base = i * 4;
		for (size_t j = 0; j < 6; ++j)
			mesh.Indices.push_back(index[j] + base);
	}
	return mesh;
}

//levels����ֱ���������slicesˮƽԲ�������
MeshData Geometry::CreateSphere(float radius, unsigned short levels, unsigned short slices, const Vector & color)
{
	MeshData meshData;
	unsigned short vertexCount = 2ui16 + levels * slices;
	//size_t indicesCount = 6 * levels * slices;
	//meshData.Vertex.resize(vertexCount);
	//meshData.Indices.resize(indicesCount);
	//��Ŷ���
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
	//����
	unsigned short base = 1;
	for (unsigned short i = 0; i < slices; ++i)
	{
		meshData.Indices.push_back(i + base);
		meshData.Indices.push_back(0);
		meshData.Indices.push_back((i + 1) % slices + base);
	}
	for (unsigned short i = 1; i < levels; ++i)
	{
		unsigned short start = 1 + i * slices;
		for (unsigned short j = 0; j < slices; ++j)
		{
			unsigned short next = (j + 1) % slices;
			meshData.Indices.push_back(start + j);
			meshData.Indices.push_back(start + j - slices);
			meshData.Indices.push_back(start + next);

			meshData.Indices.push_back(start + next);
			meshData.Indices.push_back(start + j - slices);
			meshData.Indices.push_back(start + next - slices);
		}
	}
	//�����Ӷ�������=������-���²�����
	base = vertexCount - (1 + slices);
	for (unsigned short i = 0; i < slices; ++i)
	{
		meshData.Indices.push_back(i + base);
		meshData.Indices.push_back((i + 1) % slices + base);
		meshData.Indices.push_back(vertexCount - 1);
	}
	return meshData;
}