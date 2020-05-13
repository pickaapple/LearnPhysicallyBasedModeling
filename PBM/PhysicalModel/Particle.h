#pragma once

#include "../Math/Vector.h"

class Particle
{
public:
	Particle(const Vector& pos, float mass);
	void SetPos(const Vector& pos);
	void SetPos(const float* pos, size_t size);
	void SetVelocity(const Vector& v);
	void SetVelocity(const float* v, size_t size);
	void SetForce(const Vector& f);
	void SetForce(const float* f, size_t size);
	void ResetForce();
	void AddForce(const Vector& f);
	void AddVelocity(const Vector& v);
	inline void SetMass(float m);
	inline const Vector& GetPos() const;
	inline const Vector& GetVelocity() const;
	inline  const Vector& GetForce() const;
	inline void GetAcceleration(float* a);
	inline float GetMass();
protected:
	Vector m_P;
	Vector m_V;
	Vector m_F;
	float m_Mass;
};

const Vector& Particle::GetPos() const
{
	return m_P;
}

const Vector& Particle::GetVelocity() const
{
	return m_V;
}

const Vector& Particle::GetForce() const
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

