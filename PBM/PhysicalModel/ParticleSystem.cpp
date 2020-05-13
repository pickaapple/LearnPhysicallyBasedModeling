#include "ParticleSystem.h"
#include "ODESolver.h"
#include "../Common/utility.h"
#include "../Common/Debug.h"

#include <limits>

ParticleState::ParticleState(const ParticleState & other, bool warning)
{
#if _DEBUG
	if (warning)
		Debug::LogWarning("ParticleState的赋值函数较耗时，推荐使用右值赋值构造");
#endif
	m_Length = other.m_Length;
	m_State = new float[m_Length];
	memcpy(m_State, other.m_State, m_Length * sizeof(float));
}

ParticleState::ParticleState(ParticleState && other)
{
	m_State = other.m_State;
	m_Length = other.m_Length;
	other.m_Length = 0;
	other.m_State = nullptr;
}

ParticleSystem::ParticleSystem() :
	m_GroundNormal(0, 1, 0, 0),
	m_GroundY(0),
	m_Krestitution(0.5),
	m_Kfriction(0.5)
{
}

ParticleState ParticleSystem::GetState() const
{
	size_t length = 6 * m_Particles.size();
	ParticleState state(length);
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		state.SetState(3 * 2 * i, m_Particles[i]->GetPos().data(), 3);
		state.SetState(3 * (2 * i + 1), m_Particles[i]->GetVelocity().data(), 3);
	}
	return std::move(state);
}

void ParticleSystem::SetState(const ParticleState& state)
{
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		m_Particles[i]->SetPos(state.GetState(3 * 2 * i), 3);
		m_Particles[i]->SetVelocity(state.GetState(3 * (2 * i + 1)), 3);
	}
}

ParticleState ParticleSystem::Derivative() const
{
	size_t length = 6 * m_Particles.size();
	ParticleState state(length);
	for (size_t i = 0; i < m_Particles.size(); ++i)
	{
		state.SetState(3 * 2 * i, m_Particles[i]->GetVelocity().data(), 3);
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

void ParticleSystem::CalculateForces(float deltaTime)
{
	for (auto f : m_Forces)
		f->ApplyForce(*this, deltaTime);
}

bool ParticleSystem::CollisionDetection(const ParticleState & preState, ParticleState & nextState, ODESolver& solver, float& collisionTime)
{
	Vector posInGround(0, m_GroundY, 0, 1.0f);
	//最早碰撞粒子，碰撞时间戳
	collisionTime = std::numeric_limits<float>::max();
	//碰撞检测
	for (size_t i = 0; i < nextState.Length(); i += 6)
	{
		Vector curPos(nextState[i], nextState[i + 1ull], nextState[i + 2ull], 1.0f);
		auto v = curPos - posInGround;
		float dot = Vector::Dot(v, m_GroundNormal);
		if (LT(dot, 0))
		{
			Vector prePos(preState[i], preState[i + 1ull], preState[i + 2ull], 1.0f);
			//假设碰撞体只有地面
			Vector collisionPoint = prePos + (curPos - prePos)*((m_GroundY - prePos.y()) / (curPos.y() - prePos.y()));
			ParticleState::Element collisionState{ collisionPoint.x(), collisionPoint.y(), collisionPoint.z() };
			auto time = solver.TimeUsed(&preState[i], collisionState);
			if (time < collisionTime)
				collisionTime = time;
		}
	}
	return collisionTime < std::numeric_limits<float>::max();
}

void ParticleSystem::CollisionResponse(ODESolver & solver)
{
	Vector posInGround(0, m_GroundY, 0, 1.0f);
	for (size_t i = 0; i < m_Particles.size(); ++i) {
		auto* particle = m_Particles[i];
		auto v = particle->GetPos() - posInGround;
		float dot = Vector::Dot(v, m_GroundNormal);
		if (LET(dot, 0))
		{
			//摩擦力与反作用力
			auto f = particle->GetForce();
			auto fNormal = Vector::Project(f, m_GroundNormal);
			auto friction = (f - fNormal)*-m_Kfriction;
			particle->AddForce(friction - fNormal);
			//速度反弹
			auto v = particle->GetVelocity();
			auto vNormal = Vector::Project(v, m_GroundNormal);
			auto vTangential = v - vNormal;
			particle->SetVelocity(-m_Krestitution * vNormal + vTangential);
		}
	}

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

void ParticleState::SetState(long long index, const float * state, size_t length)
{
	memcpy(m_State + index, state, length * sizeof(float));
}

void ParticleState::CopyState(long long index, float * state, size_t length) const
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

const float * ParticleState::GetState(long long index) const
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

ParticleState & ParticleState::operator=(ParticleState && other)
{
	m_State = other.m_State;
	m_Length = other.m_Length;
	other.m_Length = 0;
	other.m_State = nullptr;
	return *this;
}

float & ParticleState::operator[](size_t index) const
{
	return m_State[index];
}
