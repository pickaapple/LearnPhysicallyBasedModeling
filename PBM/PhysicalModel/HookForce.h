#pragma once
#include "Force.h"
#include "Particle.h"

class HookForce : public Force
{
public:
	HookForce(Particle* a, Particle* b);
	virtual void ApplyForce(ParticleSystem & system) override;
protected:
	float m_RestLength; //��Ϣ���ȣ����ڹ���ʱ����֮��ĳ��ȡ�
	float m_Kspring; // ����ϵ��
	float m_Kdamping; //��������ϵ��
	Particle *m_Pa, *m_Pb;
};

