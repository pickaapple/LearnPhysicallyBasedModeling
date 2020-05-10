#pragma once
#include "Force.h"
#include "Particle.h"

class HookForce : public Force
{
public:
	HookForce(Particle* a, Particle* b);
	virtual void ApplyForce(ParticleSystem & system) override;
protected:
	float m_RestLength; //静息长度，等于构造时粒子之间的长度。
	float m_Kspring; // 弹簧系数
	float m_Kdamping; //弹簧阻尼系数
	Particle *m_Pa, *m_Pb;
};

