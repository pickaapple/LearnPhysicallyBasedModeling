#pragma once
#include "Force.h"

#include "../Math/Vector.h"

class Particle; 

class DragForce : public Force
{
public:
	DragForce(Particle* p, const Vector& force);
	virtual void ApplyForce(ParticleSystem& system, float deltaTime) override;
protected:
	Particle* m_P;
	Vector m_F;
};
