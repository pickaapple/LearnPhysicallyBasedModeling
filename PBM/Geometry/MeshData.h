#pragma once
#include <vector>

#include "../Math/Vector.h"

using namespace std;

struct MeshVertex
{
	Vector Pos;
	Vector Color;
	Vector Normal;
};

//捆绑在一起的顶点
class VertexBundle
{
public:
	void SetPos(const Vector& pos);
	vector<MeshVertex*> m_Bundle;
};

class MeshData
{
public:
	vector<MeshVertex> Vertex;
	vector<unsigned short> Indices;
};

