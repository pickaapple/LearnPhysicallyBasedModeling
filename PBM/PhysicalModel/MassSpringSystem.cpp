#include "MassSpringSystem.h"
#include "../Game/World.h"
#include "../Common/utility.h"

#include <utility>

MassSpringSystem::MassSpringSystem() :
	GameObject("MassSpringSystem"),
	m_GravityValid(true)
{
	if (m_GravityValid)
		m_ParticleSystem.m_Forces.push_back(&m_Gravity);
}

void MassSpringSystem::AddMassSpring(MassSpring * element)
{
	for (auto p : element->m_Particles)
		m_ParticleSystem.m_Particles.push_back(p);
	for (auto f : element->m_Forces)
		m_ParticleSystem.m_Forces.push_back(f);
}

void MassSpringSystem::RemoveMassSpring(MassSpring * element)
{
	if (!element->m_Particles.empty())
	{
		auto &particles = m_ParticleSystem.m_Particles;
		particles.erase(remove_if(particles.begin(), particles.end(),
			[element](Particle* cur)->bool
		{
			auto &range = element->m_Particles;
			return find(range.begin(), range.end(), cur) != range.end();
		}));
	}
	if (!element->m_Forces.empty())
	{
		auto& forces = m_ParticleSystem.m_Forces;
		forces.erase(remove_if(forces.begin(), forces.end(),
			[element](Force* cur)->bool
		{
			auto &range = element->m_Forces;
			return find(range.begin(), range.end(), cur) != range.end();
		}));
	}
}

ParticleSystem & MassSpringSystem::GetParticleSystem()
{
	return m_ParticleSystem;
}

void MassSpringSystem::SetGravityValid(bool valid)
{
	if (valid == m_GravityValid)
		return;
	m_GravityValid = valid;
	if (m_GravityValid)
		m_ParticleSystem.m_Forces.push_back(&m_Gravity);
	else
		ErazeRemove(m_ParticleSystem.m_Forces, &m_Gravity);
}

void MassSpringSystem::Updated()
{
	m_ParticleSystem.ZeroParticlesForces();
	m_ParticleSystem.CalculateForces();
	auto world = World::GetInstance();
	float deltaTime = (float)(world->m_DeltaTime / 1000);
	auto curState = m_ParticleSystem.GetState();
	auto nextState = m_ODESolver.EulerStep(m_ParticleSystem, deltaTime);
	float collisionProcessTime = 0;
	float collisionTime;
	while (m_ParticleSystem.CollisionDetection(curState, nextState, m_ODESolver, collisionTime))
	{
		collisionProcessTime += collisionTime;
		if (MET(collisionProcessTime, deltaTime))
			break;
		nextState = m_ODESolver.EulerStep(m_ParticleSystem, collisionTime);
		m_ParticleSystem.SetState(nextState);
		m_ParticleSystem.CollisionResponse(m_ODESolver);
		curState = m_ParticleSystem.GetState();
		nextState = m_ODESolver.EulerStep(m_ParticleSystem, deltaTime - collisionProcessTime);
	}
	m_ParticleSystem.SetState(nextState);
	m_ParticleSystem.m_Time += deltaTime;
}
