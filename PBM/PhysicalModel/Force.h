#pragma once

class ParticleSystem;

class Force
{
public:
	virtual void ApplyForce(ParticleSystem& system, float deltaTime) = 0;
};

