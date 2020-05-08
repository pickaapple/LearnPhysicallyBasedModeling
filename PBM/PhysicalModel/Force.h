#pragma once

class ParticleSystem;

class Force
{
public:
	virtual void ApplyForce(ParticleSystem& system) = 0;
};

