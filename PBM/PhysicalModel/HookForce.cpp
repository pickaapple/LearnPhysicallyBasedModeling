#include "HookForce.h"

#include "../Common/utility.h"

HookForce::HookForce(Particle * a, Particle * b, float kSpring) :
	m_Pa(a), m_Pb(b), m_Kspring(kSpring), m_Kdamping(1),
	m_PinA(false), m_PinB(false)
{
	m_RestLength = Vector::Distance(a->GetPos(), b->GetPos());
}

void HookForce::ApplyForce(ParticleSystem & system, float deltaTime)
{
	if (m_PinA && m_PinB)
		return;
	auto Vab = m_Pa->GetPos() - m_Pb->GetPos();
	auto Dv = m_Pa->GetVelocity() - m_Pb->GetVelocity();
	auto l = Vab.Length();
	auto fa = -(m_Kspring*(l - m_RestLength) + m_Kdamping * Vector::Dot(Dv, Vab) / l)*Vab.Normalized();
	auto fb = -fa;
	if (!m_PinA)
		m_Pa->AddForce(fa);
	if (!m_PinB)
		m_Pb->AddForce(fb);
}
