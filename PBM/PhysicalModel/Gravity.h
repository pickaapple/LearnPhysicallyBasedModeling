#pragma once
#include "Force.h"
class Gravity : public Force
{
public:
	virtual void ApplyForce(ParticleSystem& system, float deltaTime) override;
	double m_GravityConstant = 9.8f;
};
