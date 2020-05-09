#include "ODESolver.h"
#include "../Common/utility.h"

#include <utility>

ParticleState ODESolver::EulerStep(const ParticleSystem& system, float deltaT)
{
	auto delta = system.Derivative();
	delta *= deltaT;
	auto curState = system.GetState();
	curState += delta;
	return std::move(curState);
}

float ODESolver::TimeUsed(ParticleState::Element preState, ParticleState::Element nextState)
{
	Vector prePos(preState[0], preState[1ull], preState[2ull], 1.0f);
	Vector preVelocity(preState[3ull], preState[4ull], preState[5ull]);
	Vector nextPos(nextState[0], nextState[1ull], nextState[2ull], 1.0f);
	return (nextPos - prePos).Length() / preVelocity.Length();
}
