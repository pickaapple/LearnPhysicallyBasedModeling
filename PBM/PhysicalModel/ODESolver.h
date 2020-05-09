#pragma once
#include "ParticleSystem.h"

class ODESolver
{
public:
	//deltaT单位为秒
	ParticleState EulerStep(const ParticleSystem& system, float deltaT);
	float TimeUsed(ParticleState::Element preState, ParticleState::Element nextState);
};

