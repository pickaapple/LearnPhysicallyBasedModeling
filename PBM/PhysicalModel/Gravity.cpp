#include "Gravity.h"
#include "ParticleSystem.h"

void Gravity::ApplyForce(ParticleSystem & system)
{
	for (auto p : system.m_Particles)
		p->AddForce({ 0, (float)(-p->GetMass() * m_GravityConstant), 0 });
}
