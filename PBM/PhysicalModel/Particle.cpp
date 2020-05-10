#include <memory>
#include <algorithm>

#include "Particle.h"


Particle::Particle(const Vector& pos, float mass) :
	m_P(pos), m_Mass(mass)
{
}

void Particle::SetPos(const Vector & pos)
{
	m_P = pos;
}

void Particle::SetPos(const float * pos, size_t size)
{
	m_P.SetData(pos, size);
}

void Particle::SetVelocity(const Vector & v)
{
	m_V = v;
}

void Particle::SetVelocity(const float * v, size_t size)
{
	m_V.SetData(v, size);
}

void Particle::SetForce(const Vector & f)
{
	m_F = f;
}

void Particle::SetForce(const float * f, size_t size)
{
	m_F.SetData(f, size);
}

void Particle::ResetForce()
{
	m_F = Vector::Zero;
}

void Particle::AddForce(const Vector & f)
{
	m_F += f;
}

