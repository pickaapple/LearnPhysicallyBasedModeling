#include "Matrix.h"
#include "Vector.h"

#include <algorithm>
#include <cmath>

using namespace DirectX;

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

float Vector::Length() const
{
	return std::sqrtf(XMVectorGetX(XMVector3Dot(m_V, m_V)));
}

void Vector::Normalize()
{
	m_V = XMVector3Normalize(m_V);
}

void Vector::Project(const Vector & on)
{
	*this = Vector::Project(*this, on);
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

float Vector::operator[](int index) const
{
	if (index < 0)
		index = 4 - abs(index) % 4;
	switch (index) {
	case 0:return x();
	case 1:return y();
	case 2:return z();
	case 3:return w();
	}
	throw new out_of_range("索引超出范围");
}

Vector operator+(const Vector & a, const Vector & b)
{
	return XMVectorAdd(a.m_V, b.m_V);
}

Vector operator-(const Vector & a, const Vector & b)
{
	return XMVectorSubtract(a.m_V, b.m_V);
}

Vector operator*(const Vector & v, float scale)
{
	return Vector(v.x()*scale, v.y()*scale, v.z()*scale, v.w()*scale);
}

Vector operator*(float scale, const Vector & v)
{
	return v * scale;
}

Vector operator*(const Vector & v, const Matrix & m)
{
	return XMVector4Transform(v.m_V, m.m_M);
}

Vector Vector::Cross(const Vector& a, const Vector& b)
{
	auto m = XMVector3Cross(a.m_V, b.m_V);
	return m;
}

Vector Vector::Project(const Vector & v, const Vector & on)
{
	return (Vector::Dot(v, on) / std::pow(on.Length(), 2)) * on;
}

float Vector::Dot(const Vector & a, const Vector & b)
{
	return XMVectorGetX(XMVector3Dot(a.m_V, b.m_V));
}

float Vector::Distance(const Vector & a, const Vector & b)
{
	auto delta = a - b;
	return delta.Length();
}

void Vector::SetData(const float * pos, size_t size)
{
	memcpy(&m_V, pos, std::min(size, (size_t)4) * sizeof(float));
}

Vector Vector::Forward(0, 0, 1, 0);
Vector Vector::Up(0, 1, 0, 0);
Vector Vector::Right(1, 0, 0, 0);
Vector Vector::Zero;
Vector Vector::OnePosition(1, 1, 1, 1);
Vector Vector::OneVector(1, 1, 1, 0);
