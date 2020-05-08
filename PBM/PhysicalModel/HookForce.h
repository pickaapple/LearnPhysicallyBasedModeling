#pragma once
#include "Force.h"
#include "Particle.h"

class HookForce : public Force
{
public:
	HookForce(Particle* a, Particle* b);
	virtual void ApplyForce(ParticleSystem & system) override;
protected:
	Particle *m_Pa, *m_Pb;
};

