#pragma once
#include "../stdafx.h"
#include "Particle.h"
#include "Force.h"

class ParticleState
{
public:
	ParticleState(ParticleState&& other);
	ParticleState(size_t length);
	~ParticleState();
	void SetState(int index, const float* state, size_t length);
	const float* GetState(int index) const;
	void CopyState(int index, float* state, size_t length) const;
	string ToString() const;
	ParticleState& operator *= (float scale);
	ParticleState& operator +=(ParticleState& other);
protected:
	float* m_State;
	size_t m_Length;
};


class ParticleSystem
{
public:
	ParticleState GetState();
	void SetState(const ParticleState& state);
	ParticleState Derivative();
	void ZeroParticlesForces();
	void CalculateForces();
	vector<Particle*> m_Particles;
	vector<Force*> m_Forces;
	double m_Time;
};