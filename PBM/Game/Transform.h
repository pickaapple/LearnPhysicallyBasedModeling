#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();

	void Move(const Vector& vector);
	void Move(float delta_x, float delta_y, float delta_z = 0);

	void Roate(double degree[3]);
	inline void Roate(double degree_x, double degree_y, double degree_z);
	inline void RoateX(double degree);
	inline void RoateY(double degree);
	inline void RoateZ(double degree);
	void RoatePositionByAxis(const Vector& position, const Vector& axis, float radian);

	void SetMatrix(const Matrix& rotateMatrix);
	inline Matrix GetMatrix() const;

	inline const Vector& GetPosition() const;
	inline void SetPosition(const Vector& position);
	Vector ForwardDirection() const;
	Vector UpDirection() const;
	Vector RightDirection() const;
protected:
	Vector m_aPosition = { 0.0f ,0.0f, 0.0f, 1.0f };
	Matrix m_RotateMatrix;
};

void Transform::Roate(double degree_x, double degree_y, double degree_z)
{
	double roate[3]{ degree_x, degree_y, degree_z };
	Roate(roate);
}

void Transform::RoateX(double degree)
{
	double roate[3]{ degree, 0, 0 };
	Roate(roate);
}

void Transform::RoateY(double degree)
{
	double roate[3]{ 0, degree, 0 };
	Roate(roate);
}

void Transform::RoateZ(double degree)
{
	double roate[3]{ 0, 0, degree };
	Roate(roate);
}

Matrix Transform::GetMatrix() const
{
	auto translate = Matrix::Translate(m_aPosition);
	return Matrix::Multiply(m_RotateMatrix, translate);
}

inline const Vector& Transform::GetPosition() const
{
	return m_aPosition;
}

inline void Transform::SetPosition(const Vector & position)
{
	m_aPosition = position;
}
