#include "DivForce.h"

DivForce::DivForce()
{
}

DivForce::DivForce(FunDivForceApply funApply) :
	m_funApplyForce(funApply)
{
}

void DivForce::ApplyForce(ParticleSystem & system, float deltaTime)
{
	if (m_funApplyForce)
		m_funApplyForce(m_P, system);
}
