#pragma once
#include <directxmath.h>
#include <string>

using namespace std;
using namespace DirectX;

struct Matrix;

struct Vector
{
public:
	Vector();
	Vector(const XMVECTOR& v);
	Vector(float x, float y, float z, float w = 0);
	float x() const;
	float y() const;
	float z() const;
	float w() const;
	void Normalize();
	Vector Normalized() const;

	string ToString() const;

	Vector operator-() const;
	Vector& operator *=(const Matrix& matrix);
	Vector& operator +=(const Vector& other);
	
	static Vector Cross(Vector a, Vector b);

	const float* data() const;
	void SetData(const float* pos, size_t size);

	friend Vector operator+(const Vector& a, const Vector& b);
	friend Vector operator-(const Vector& a, const Vector& b);
	friend Vector operator*(const Vector& v, float scale);
	friend Vector operator*(const Vector& v, const Matrix& m);
public:
	static Vector Forward;
	static Vector Up;
	static Vector Right;
	XMVECTOR m_V;
};

