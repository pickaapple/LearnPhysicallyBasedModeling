#pragma once
#include "ParticleSystem.h"

class ODESolver
{
public:
	//deltaT��λΪ��
	void EulerStep(ParticleSystem& system, double deltaT);
};

