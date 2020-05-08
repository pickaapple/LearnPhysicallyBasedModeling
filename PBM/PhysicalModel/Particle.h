#pragma once

class Particle
{
public:
	Particle(const float* pos, float mass);
	inline float* GetPos();
	void SetPos(const float* pos);
	inline float* GetVelocity();
	void SetVelocity(const float* v);
	inline const float* GetForce();
	void SetForce(const float* f);
	void ResetForce();
	inline void GetAcceleration(float* a);
	inline float GetMass();
	inline void SetMass(float m);
protected:
	float m_P[3];
	float m_V[3];
	float m_F[3];
	float m_Mass;
};

float* Particle::GetPos()
{
	return m_P;
}

inline float * Particle::GetVelocity()
{
	return m_V;
}

inline const  float * Particle::GetForce()
{
	return m_F;
}

inline void Particle::GetAcceleration(float * a)
{
	a[0] = m_F[0] / m_Mass;
	a[1] = m_F[1] / m_Mass;
	a[2] = m_F[2] / m_Mass;
}

inline float Particle::GetMass()
{
	return m_Mass;
}

inline void Particle::SetMass(float m)
{
	m_Mass = m;
}

