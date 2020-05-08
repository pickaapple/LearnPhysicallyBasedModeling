#include "Matrix.h"
#include "Vector.h"

Matrix Matrix::Identity(XMMatrixIdentity());

Matrix::Matrix(const XMMATRIX & m) :
	m_M(m)
{
}

Matrix::Matrix(const Matrix & m) :
	m_M(m.m_M)
{
}

Matrix::Matrix(Vector r0, Vector r1, Vector r2, Vector r3) :
	m_M(r0.m_V, r1.m_V, r2.m_V, r3.m_V)
{
}

void Matrix::Transpose()
{
	m_M = XMMatrixTranspose(m_M);
}

bool Matrix::Inverse()
{
	auto m = XMMatrixInverse(nullptr, m_M);
	if (XMMatrixIsNaN(m))
		return false;
	m_M = m;
	return true;
}

Matrix Matrix::Inversed() const
{
	return XMMatrixInverse(nullptr, m_M);
}

Matrix Matrix::Transposed() const
{
	return XMMatrixTranspose(m_M);
}

Matrix Matrix::Multiply(const Matrix& a, const Matrix& b)
{
	return XMMatrixMultiply(a.m_M, b.m_M);
}

Matrix Matrix::Translate(const Vector & v)
{
	return XMMatrixTranslation(v.x(), v.y(), v.z());
}

Matrix Matrix::RoateMatrix(double radian[3])
{

	double sinx = sin(radian[0]), cosx = cos(radian[0]),
		siny = sin(radian[1]), cosy = cos(radian[1]),
		sinz = sin(radian[2]), cosz = cos(radian[2]);
	double cosz_siny = cosz * siny;
	double sinz_siny = sinz * siny;
	// Rz * Ry * Rx
	auto m = XMMATRIX({
		(float)(cosz*cosy), (float)(sinz*cosx + cosz_siny * sinx), (float)(sinz*sinx - cosz_siny * cosx), 0,
		(float)(-sinz * cosy), (float)(cosz*cosx - sinz_siny * sinx), (float)(cosz*sinx + sinz_siny * cosx), 0,
		(float)(siny), (float)(-cosy * sinx), (float)(cosy*cosx), 0,
		0,0,0,1
		});
	return m;
}

Matrix Matrix::RoateAxisMatrix(const Vector & axis, float radian)
{
	return XMMatrixRotationAxis(axis.m_V, radian);
}

Matrix Matrix::RoateAxisMatrix(const Vector & position, const Vector & axis, float radian)
{
	//先将position移置原点，再旋转，再移回
	auto matrix = Matrix::Translate(-position);
	matrix = Matrix::Multiply(matrix, RoateAxisMatrix(axis, radian));
	matrix = Matrix::Multiply(matrix, Matrix::Translate(position));
	return matrix;
}

Matrix Matrix::LookToLH(const Vector & eye, const Vector & forward, const Vector & up)
{
	return XMMatrixLookToLH(eye.m_V, forward.m_V, up.m_V);
}

Matrix Matrix::PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	return XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
}

Matrix operator*(const Matrix & a, const Matrix & b)
{
	return Matrix::Multiply(a, b);
}
