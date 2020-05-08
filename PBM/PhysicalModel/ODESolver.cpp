#include "ODESolver.h"
#include "../Common/utility.h"

void ODESolver::EulerStep(ParticleSystem& system, double deltaT)
{
	auto delta = system.Derivative();
	delta *= deltaT;
	auto curState = system.GetState();
	curState += delta;
	system.SetState(curState);
	//Debug::Log("State " + curState.ToString());
	system.m_Time += deltaT;

}
