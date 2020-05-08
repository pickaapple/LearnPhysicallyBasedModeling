#pragma once
#include "ParticleSystem.h"

class ODESolver
{
public:
	//deltaTµ¥Î»ÎªÃë
	void EulerStep(ParticleSystem& system, double deltaT);
};

