#pragma once
#include "Force.h"
#include "Particle.h"

class HookForce : public Force
{
public:
	HookForce(Particle* a, Particle* b, float kSpring=1.0f);
	virtual void ApplyForce(ParticleSystem & system, float deltaTime) override;
public:
	float m_Kspring; // 弹簧系数
	float m_Kdamping; //弹簧阻尼系数
	bool m_PinA; //固定a端
	bool m_PinB; //固定b端
protected:
	float m_RestLength; //静息长度，等于构造时粒子之间的长度。
	Particle *m_Pa, *m_Pb;
};

