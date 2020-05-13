#include "DragForce.h"
#include "Particle.h"

DragForce::DragForce(Particle * p, const Vector& f) :
	m_P(p), m_F(f)
{
}

void DragForce::ApplyForce(ParticleSystem & system, float deltaTime)
{
	m_P->AddForce(m_F);
}
