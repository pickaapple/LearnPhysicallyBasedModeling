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
public:
	Gravity m_Gravity;
protected:
	virtual void Updated() override;
	ODESolver solver;
	ParticleSystem m_ParticleSystem;
};

