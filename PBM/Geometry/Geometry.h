#pragma once
#include "MeshData.h"
class Geometry
{
public:
	static MeshData CreateBox(float width, float height, float depth, const Vector& color);
	//levels����ֱ���������slicesˮƽԲ�������
	static MeshData CreateSphere(float radius, unsigned short levels, unsigned short slices, const  Vector& color);
};

