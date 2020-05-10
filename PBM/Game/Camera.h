#pragma once
#include "GameObject.h"

enum ECameraMode {
	CAMERA_MODE_NONE,  // 静止
	FIRST_PLAYER,  // 第一人称
	THIRD_PLAYER   //  第三人称
};

struct ThirdPlayerInfo {
	double Distance;
	double MaxDistance;
	double MinDistance;
};

class Camera : public GameObject, public CallKeyStateChange
{
public:
	Camera(const string& name);
	inline Matrix ViewMatrix() const;
	inline Matrix ProjectionMatrix() const;
	void SetCameraMode(ECameraMode mode, GameObject* target);
	void SetPosition(const Vector& eyePosition);
	void AdjustCameraDirection(Vector focusPosition, Vector upDirection);
	void LookTo(const Vector& eyePosition, Vector focusPosition, Vector upDirection);
protected:
	virtual void Updated() override;
	float GetHRadian(int deltaX, double deltaTime);
	float GetVRadian(int deltaY, double deltaTime);
protected:
	GameObject *m_pTargetGO;
	ECameraMode m_Mode;
	double m_VAngularVelocity = ToRadian(60) / 1000.0;  // 垂直角速度 单位 : 弧度/ms
	double m_MaxVAngularVelocity = ToRadian(360) / 1000.0;
	double m_HAngularVelocity = ToRadian(60) / 1000.0;  // 水平角速度 单位 : 弧度/ms
	double m_MaxHAngularVelocity = ToRadian(360) / 1000.0;
	float fovAngleY = (float)ToRadian(60.0);
	float aspectRatio = 1;
	float nearZ = 5;
	float farZ = 30;
	ThirdPlayerInfo m_ThirdPlayerInfo;
	bool m_MouseCoordInvalid;
	POINT m_MouseCoord;

	virtual void CallKeyStateChange::KeyboardStateChange(KeyCode code, KEY_STATE state) override;
};

Matrix Camera::ViewMatrix() const
{
	const auto& eye = m_Transform.GetPosition();
	const auto& forward = m_Transform.ForwardDirection();
	const auto& up = m_Transform.UpDirection();
	return Matrix::LookToLH(eye, forward, up);
}

Matrix Camera::ProjectionMatrix() const
{
	return Matrix::PerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
}
