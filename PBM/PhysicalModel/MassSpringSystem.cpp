#include "MassSpringSystem.h"
#include "../Game/World.h"

MassSpringSystem::MassSpringSystem() :
	GameObject("MassSpringSystem")
{
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
	auto &particles = m_ParticleSystem.m_Particles;
	particles.erase(remove_if(particles.begin(), particles.end(),
		[element](Particle* cur)->bool
	{
		auto &range = element->m_Particles;
		return find(range.begin(), range.end(), cur) != range.end();
	}));

	auto& forces = m_ParticleSystem.m_Forces;
	forces.erase(remove_if(forces.begin(), forces.end(),
		[element](Force* cur)->bool
	{
		auto &range = element->m_Forces;
		return find(range.begin(), range.end(), cur) != range.end();
	}));
}

ParticleSystem & MassSpringSystem::GetParticleSystem()
{
	return m_ParticleSystem;
}

void MassSpringSystem::Updated()
{
	m_ParticleSystem.ZeroParticlesForces();
	m_ParticleSystem.CalculateForces();
	auto world = World::GetInstance();
	double deltaTime = world->m_DeltaTime / 1000;
	solver.EulerStep(m_ParticleSystem, deltaTime);
}