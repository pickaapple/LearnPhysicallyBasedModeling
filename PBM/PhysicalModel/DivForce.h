#pragma once

#include "Force.h"

class Particle;

typedef void (*FunDivForceApply)(Particle* particle, ParticleSystem& system);

class DivForce : public Force
{
public:
	DivForce();
	DivForce(FunDivForceApply fun);
	virtual void ApplyForce(ParticleSystem & system, float deltaTime) override;
public:
	Particle* m_P;
protected:
	FunDivForceApply m_funApplyForce;
};
