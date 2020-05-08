#include <memory>

#include "Particle.h"


Particle::Particle(const float * pos, float mass)
{
	memcpy(m_P, pos, 3 * sizeof(float));
	memset(m_V, 0, 3 * sizeof(float));
	memset(m_F, 0, 3 * sizeof(float));
	m_Mass = mass;
}

void Particle::SetPos(const float * pos)
{
	memcpy(m_P, pos, 3 * sizeof(float));
}

void Particle::SetVelocity(const float * v)
{
	memcpy(m_V, v, 3 * sizeof(float));
}

void Particle::SetForce(const float * f)
{
	memcpy(m_F, f, 3 * sizeof(float));
}

void Particle::ResetForce()
{
	memset(m_F, 0, 3 * sizeof(float));
}

