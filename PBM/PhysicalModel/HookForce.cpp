#include "HookForce.h"

HookForce::HookForce(Particle * a, Particle * b)
	:m_Pa(a), m_Pb(b)
{
}

void HookForce::ApplyForce(ParticleSystem & system)
{
}
