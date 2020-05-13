#pragma once
#include "MeshData.h"
#include "MassSpringData.h"

class Geometry
{
public:
	static MeshData CreateBox(float width, float height, float depth, const Vector& color);
	static MassSpringData CreateBoxMS(float width, float height, float depth, float mass, float spring);
	//levels：垂直切面个数，slices水平圆顶点个数
	static MeshData CreateSphere(float radius, unsigned short levels, unsigned short slices, const  Vector& color);

	static MeshData CreateGrid(float length, unsigned short count, const  Vector& color);
	static MeshData CreateCircle(float radius, const Vector& center, unsigned short slices, const Vector& color);
};
