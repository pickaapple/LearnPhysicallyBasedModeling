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
	Vector xyz() const;
	float w() const;
	float Length() const;
	void Normalize();
	void Project(const Vector& on);
	Vector Normalized() const;

	string ToString() const;
	bool Equals(const Vector& other) const;

	Vector operator-() const;
	Vector& operator *=(const Matrix& matrix);
	Vector& operator +=(const Vector& other);
	float operator[](int index) const;

	static Vector Cross(const Vector& a, const Vector& b);
	static Vector Project(const Vector& v, const Vector& on);
	static float Dot(const Vector& a, const Vector& b);
	static float Distance(const Vector& a, const Vector& b);

	inline float* data() const;
	void SetData(const float* pos, size_t size);

	friend Vector operator+(const Vector& a, const Vector& b);
	friend Vector operator-(const Vector& a, const Vector& b);
	friend Vector operator*(const Vector& v, float scale);
	friend Vector operator*(float scale, const Vector& v);
	friend Vector operator*(const Vector& v, const Matrix& m);
public:
	static Vector Forward;
	static Vector Up;
	static Vector Right;
	static Vector Zero;
	static Vector OnePosition;
	static Vector OneVector;
	static Vector Red;
	static Vector Green;
	static Vector Blue;
	static Vector Gray;
	XMVECTOR m_V;
};

float * Vector::data() const
{
	return (float*)&m_V;
}

typedef Vector Color;
