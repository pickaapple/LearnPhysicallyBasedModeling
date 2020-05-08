#include "Matrix.h"
#include "Vector.h"
#include <algorithm>

Vector::Vector() :
	m_V(XMVectorZero())
{
}

Vector::Vector(const XMVECTOR & v) :
	m_V(v)
{
}

Vector::Vector(float x, float y, float z, float w) :
	m_V(XMVectorSet(x, y, z, w))
{

}

float Vector::x() const
{
	return XMVectorGetX(m_V);
}

float Vector::y() const
{
	return XMVectorGetY(m_V);
}

float Vector::z() const
{
	return XMVectorGetZ(m_V);
}

float Vector::w() const
{
	return XMVectorGetW(m_V);
}

void Vector::Normalize()
{
	m_V = XMVector3Normalize(m_V);
}

Vector Vector::Normalized() const
{
	return XMVector3Normalize(m_V);
}

string Vector::ToString() const
{
	auto info = "x:" + to_string(x()) + ";y:" + to_string(y()) + ";z:" + to_string(z()) + ";w:" + to_string(w());
	return  info;
}

Vector Vector::operator-() const
{
	return Vector(-x(), -y(), -z(), w());
}

Vector & Vector::operator*=(const Matrix & matrix)
{
	auto v = (*this) * matrix;
	m_V = v.m_V;
	return *this;
}

Vector & Vector::operator+=(const Vector & other)
{
	m_V = XMVectorAdd(m_V, other.m_V);
	return *this;
}

Vector operator+(const Vector & a, const Vector & b)
{
	return Vector(XMVectorAdd(a.m_V, b.m_V));
}

Vector operator-(const Vector & a, const Vector & b)
{
	return Vector(XMVectorSubtract(a.m_V, b.m_V));
}

Vector operator*(const Vector & v, float scale)
{
	return Vector(v.x()*scale, v.y()*scale, v.z()*scale, v.w()*scale);
}


Vector operator*(const Vector & v, const Matrix & m)
{
	return Vector(XMVector4Transform(v.m_V, m.m_M));
}

Vector Vector::Cross(Vector a, Vector b)
{
	auto m = XMVector3Cross(a.m_V, b.m_V);
	return Vector(m);
}

const float * Vector::data() const
{
	return m_V.m128_f32;
}

void Vector::SetData(const float * pos, size_t size)
{
	size = std::min(size, (size_t)4);
	if (size > 0)
		memcpy(&m_V, pos, size * sizeof(float));
}

Vector Vector::Forward(0, 0, 1, 0);
Vector Vector::Up(0, 1, 0, 0);
Vector Vector::Right(1, 0, 0, 0);