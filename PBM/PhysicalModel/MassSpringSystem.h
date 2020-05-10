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
protected:
	virtual void Updated() override;
public:
	Gravity m_Gravity;
protected:
	ODESolver m_ODESolver;
	ParticleSystem m_ParticleSystem;
};

