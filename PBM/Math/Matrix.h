#pragma once
#include <directxmath.h>

using namespace DirectX;

struct Vector;

struct Matrix
{
public:
	Matrix(const XMMATRIX& m);
	Matrix(const Matrix& m);
	Matrix(Vector r0, Vector r1, Vector r2, Vector r3);

	void Transpose();
	//�������ʽΪ��Ϊ0�����淴���󡣷����淴���ҷ���false��
	bool Inverse();

	Matrix Transposed() const;
	Matrix Inversed() const;

	static Matrix Multiply(const Matrix& a, const Matrix& b);
	static Matrix Translate(const Vector& v);
	static Matrix RoateMatrix(double radian[3]);
	static Matrix RoateAxisMatrix(const Vector&axis, float radian);
	static Matrix RoateAxisMatrix(const Vector& position, const Vector&axis, float radian);

	static Matrix LookToLH(const Vector& eye, const Vector& forward, const Vector& up);
	static Matrix PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	friend Matrix operator*(const Matrix& a, const Matrix& b);

public:
	static Matrix Identity;
	XMMATRIX m_M;
};

