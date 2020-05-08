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

class MeshData
{
public:
	vector<MeshVertex> Vertex;
	vector<unsigned short> Indices;
};

