#pragma once
#include "Force.h"
class Gravity : public Force
{
public:
	virtual void ApplyForce(ParticleSystem& system) override;
	double m_GravityConstant = 9.8f;
};
