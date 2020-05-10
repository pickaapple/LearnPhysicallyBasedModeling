#include "Transform.h"
#include "GameObject.h"

Transform::Transform() :
	m_RotateMatrix(Matrix::Identity)
{
}

void Transform::Move(const Vector & vector)
{
	m_aPosition += vector;
}

void Transform::Move(float delta_x, float delta_y, float delta_z)
{
	Move({ delta_x, delta_y, delta_z });
}

void Transform::Roate(double degree[3])
{
	//cout << gameObject->m_Name.data() << " Roate " << degree[0] << ", " << degree[1] << "," << degree[2] << endl;
	double radian[3]{ ToRadian(degree[0]), ToRadian(degree[1]), ToRadian(degree[2]) };
	m_RotateMatrix = m_RotateMatrix * Matrix::RoateMatrix(radian);
}

void Transform::RoatePositionByAxis(const Vector& position, const Vector& axis, float radian)
{
	//Debug::Log(gameObject->m_Name + "  RoateAxis position: " + position.ToString() + " axis:" + axis.ToString());
	m_aPosition *= Matrix::RoateAxisMatrix(position, axis, radian);
}

void Transform::SetMatrix(const Matrix& rotateMatrix)
{
	m_RotateMatrix = rotateMatrix;
}

Vector Transform::ForwardDirection() const
{
	return Vector::Forward * m_RotateMatrix;
}

Vector Transform::UpDirection() const
{
	return Vector::Up * m_RotateMatrix;
}

Vector Transform::RightDirection() const
{
	return Vector::Right * m_RotateMatrix;
}