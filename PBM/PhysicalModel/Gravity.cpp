#include "Gravity.h"
#include "ParticleSystem.h"

void Gravity::ApplyForce(ParticleSystem & system)
{
	for (auto p : system.m_Particles)
	{
		float f[3]{ 0, p->GetForce()[1] - p->GetMass() * m_GravityConstant, 0 };
		p->SetForce(f);
	}
}
