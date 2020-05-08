#include "Camera.h"
#include "../Windows/Keyboard.h"
#include "../Windows/Mouse.h"
#include "World.h"

Camera::Camera(const string& name) :
	GameObject(name),
	m_ThirdPlayerInfo({ 20.0, 25.0, 10.0 })
{
	LookTo({ 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 0 }, { 0.0, 1.0, 0.0,0 });
}

void Camera::SetCameraMode(ECameraMode mode, GameObject* target)
{
	m_pTargetGO = target;
	m_MouseCoordInvalid = true;

	auto tPosition = m_pTargetGO->m_Transform.GetPosition();
	auto tUpDirection = m_pTargetGO->m_Transform.UpDirection();
	auto tForwarDirection = m_pTargetGO->m_Transform.ForwardDirection();
	auto tRIghtDirection = m_pTargetGO->m_Transform.RightDirection();
	// 设置照相机的初始位置
	switch (mode) {
	case FIRST_PLAYER: {
		float carHeight = 4.5;
		float carLength = 6;
		auto eyePosition = tPosition + tUpDirection * carHeight + tForwarDirection * carLength;
		LookTo(eyePosition, tForwarDirection, tUpDirection);
		break;
	}
	case THIRD_PLAYER: {
		auto eyePosition = tPosition - tForwarDirection * (float)m_ThirdPlayerInfo.Distance;
		LookTo(eyePosition, tForwarDirection, tUpDirection);
		//m_Transform.RoatePositionByAxis(tPosition, tRIghtDirection, (float)ToRadian(45));
		break;
	}
	default:
		break;
	}
	m_Mode = mode;
}

void Camera::Updated()
{
	if (!m_pTargetGO) return;
	auto pWorld = World::GetInstance();
	double deltaTime = pWorld->m_DeltaTime;
	auto& mouse = *pWorld->m_pMouse;
	auto tPosition = m_pTargetGO->m_Transform.GetPosition();
	// 设置鼠标输入信息

	if (mouse.IsTracking())
	{
		if (m_MouseCoordInvalid) {
			m_MouseCoordInvalid = false;
			m_MouseCoord = mouse.VirtualCoord();
			return;
		}
		POINT curMouseCoord = mouse.VirtualCoord();
		POINT delta = PointSubstract(curMouseCoord, m_MouseCoord);
		m_MouseCoord = curMouseCoord;
		//水平方向上移动
		if (abs(delta.x) > 20) {
			auto hRadian = GetHRadian(delta.x, deltaTime);
			//cout << "Third Player roate surrounding horizontal degree: " << ToDegree(hRadian) << endl;
			if (m_Mode == THIRD_PLAYER)
				m_Transform.RoatePositionByAxis(tPosition, Vector::Up, hRadian);
			else if (m_Mode == FIRST_PLAYER)
				m_Transform.RoatePositionByAxis(m_Transform.GetPosition(), Vector::Up, hRadian);
		}
		if (abs(delta.y) > 20) {
			auto vRadian = GetVRadian(delta.y, deltaTime);
			//cout << "Third Player roate surrounding vertical degree: " << ToDegree(vRadian) << endl;
			if (m_Mode == THIRD_PLAYER)
				m_Transform.RoatePositionByAxis(tPosition, m_Transform.RightDirection(), vRadian);
			else if (m_Mode == FIRST_PLAYER)
				m_Transform.RoatePositionByAxis(m_Transform.GetPosition(), m_Transform.RightDirection(), vRadian);
		}
		//滚轮滑动
		if (m_Mode == THIRD_PLAYER) {
			double wheelDistance = mouse.WheelDistance();
			if (abs(wheelDistance) > FLT_EPSILON) {
				double distance = m_ThirdPlayerInfo.Distance + -wheelDistance * 1.5;
				if (distance <= m_ThirdPlayerInfo.MaxDistance && distance >= m_ThirdPlayerInfo.MinDistance) {
					m_ThirdPlayerInfo.Distance = distance;
				}
				mouse.ResetWheel();
			}
		}
	}

	switch (m_Mode)
	{
	case CAMERA_MODE_NONE:
		break;
	case FIRST_PLAYER: {
		auto tUpDirection = m_pTargetGO->m_Transform.UpDirection();
		auto tForwarDirection = m_pTargetGO->m_Transform.ForwardDirection();
		float carHeight = 4.5;
		float carLength = 6;
		auto newEyePosition = tPosition + tUpDirection * carHeight + tForwarDirection * carLength;
		auto lastEyePosition = m_Transform.GetPosition();
		auto deltaVector = newEyePosition - lastEyePosition;
		m_Transform.Move(deltaVector.x(), deltaVector.y(), deltaVector.z());
		break;
	}
	case THIRD_PLAYER: {
		// 相机相对于目标的方位
		auto focusDirection = tPosition - m_Transform.GetPosition();
		AdjustCameraDirection(focusDirection, { 0,1,0,0 });
		break;
	}
	}
}

void Camera::SetPosition(const Vector & eyePosition)
{
	m_Transform.SetPosition(eyePosition);
}


void Camera::AdjustCameraDirection(Vector focusDirection, Vector upDirection)
{
	focusDirection.Normalize();
	upDirection.Normalize();
	auto rightDirection = Vector::Cross(upDirection, focusDirection);
	upDirection = Vector::Cross(focusDirection, rightDirection);
	XMMATRIX matrix(rightDirection.m_V, upDirection.m_V, focusDirection.m_V, { 0,0,0,1 });

	m_Transform.SetMatrix(matrix);
}

void Camera::LookTo(const Vector & eyePosition, Vector focusDirection, Vector upDirection)
{
	SetPosition(eyePosition);
	AdjustCameraDirection(focusDirection, upDirection);
}

float Camera::GetHRadian(int deltaX, double deltaTime)
{
	//水平方向上转动
	float deltaXStage = 0;
	if (abs(deltaX) > 4)
		deltaXStage = 1;
	else if (abs(deltaX) > 8)
		deltaXStage = 3;
	else
		return 0.0;
	float hRadian = (float)(m_HAngularVelocity * deltaTime * Sign(deltaX) * deltaXStage);
	float hMaxRadian = (float)(m_MaxHAngularVelocity * deltaTime);
	if (abs(hRadian) > hMaxRadian)
		hRadian = hMaxRadian * Sign(hRadian);
	return hRadian;
}

float Camera::GetVRadian(int deltaY, double deltaTime)
{
	//垂直方向上转动
	float deltaYStage = 0;
	if (abs(deltaY) > 4)
		deltaYStage = 1;
	else if (abs(deltaY) > 8)
		deltaYStage = 3;
	else
		return 0.0;
	float vRadian = (float)(m_VAngularVelocity * deltaTime * Sign(deltaY) * deltaYStage);
	float vMaxRadian = (float)(m_MaxVAngularVelocity * deltaTime);
	if (abs(vRadian) > vMaxRadian)
		vRadian = vMaxRadian * Sign(vRadian);
	return vRadian;
}

void Camera::KeyboardStateChange(KeyCode code, KEY_STATE state)
{
	if (code == KEY_R) {
		if (state == KEY_STATE_DOWN) {
			if (m_Mode == THIRD_PLAYER)
				SetCameraMode(FIRST_PLAYER, m_pTargetGO);
			else
				SetCameraMode(THIRD_PLAYER, m_pTargetGO);
		}
	}
}
