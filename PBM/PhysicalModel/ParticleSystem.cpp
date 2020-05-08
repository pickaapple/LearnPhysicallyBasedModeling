#include "ParticleSystem.h"

ParticleState::ParticleState(ParticleState && other)
{
	m_State = other.m_State;
	m_Length = other.m_Length;
	other.m_Length = 0;
	other.m_State = nullptr;
}

ParticleState ParticleSystem::GetState()
{
	size_t length = 6 * m_Particles.size();
	ParticleState state(length);
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		state.SetState(3 * 2 * i, m_Particles[i]->GetPos(), 3);
		state.SetState(3 * (2 * i + 1), m_Particles[i]->GetVelocity(), 3);
	}
	return std::move(state);
}

void ParticleSystem::SetState(const ParticleState& state)
{
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		m_Particles[i]->SetPos(state.GetState(3 * 2 * i));
		m_Particles[i]->SetVelocity(state.GetState(3 * (2 * i + 1)));
	}
}

ParticleState ParticleSystem::Derivative()
{
	size_t length = 6 * m_Particles.size();
	ParticleState state(length);
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		state.SetState(3 * 2 * i, m_Particles[i]->GetVelocity(), 3);
		float a[3];
		m_Particles[i]->GetAcceleration(a);
		state.SetState(3 * (2 * i + 1), a, 3);
	}
	return std::move(state);
}

void ParticleSystem::ZeroParticlesForces()
{
	for (auto p : m_Particles)
		p->ResetForce();
}

void ParticleSystem::CalculateForces()
{
	for (auto f : m_Forces)
		f->ApplyForce(*this);
}

ParticleState::ParticleState(size_t length)
{
	m_State = new float[length];
	m_Length = length;
}

ParticleState::~ParticleState()
{
	if (m_State) delete[] m_State;
}

void ParticleState::SetState(int index, const float * state, size_t length)
{
	memcpy(m_State + index, state, length * sizeof(float));
}

void ParticleState::CopyState(int index, float * state, size_t length) const
{
	memcpy(state, m_State + index, length * sizeof(float));
}

string ParticleState::ToString() const
{
	string ans;
	for (size_t i = 0; i < m_Length; ++i)
		ans += to_string(m_State[i]) + " ";
	return std::move(ans);
}

const float * ParticleState::GetState(int index) const
{
	return m_State + index;
}

ParticleState & ParticleState::operator*=(float scale)
{
	for (size_t i = 0; i < m_Length; ++i)
		m_State[i] *= scale;
	return *this;
}

ParticleState & ParticleState::operator+=(ParticleState & other)
{
	for (size_t i = 0; i < m_Length && i < other.m_Length; ++i)
		m_State[i] += other.m_State[i];
	return *this;
}
