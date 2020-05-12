#pragma once
#include "../Game/GameObject.h"
#include "MassSpring.h"
#include "Gravity.h"
#include "ParticleSystem.h"
#include "ODESolver.h"
class MassSpringSystem : public GameObject
{
public:
	MassSpringSystem();
	void AddMassSpring(MassSpring* element);
	void RemoveMassSpring(MassSpring* element);
	ParticleSystem& GetParticleSystem();
	void SetGravityValid(bool valid);
protected:
	virtual void Updated() override;
public:
	Gravity m_Gravity;
protected:
	bool m_GravityValid;
	ODESolver m_ODESolver;
	ParticleSystem m_ParticleSystem;
};

