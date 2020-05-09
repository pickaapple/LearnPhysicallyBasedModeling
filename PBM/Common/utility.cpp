#include "../stdafx.h"
#include "utility.h"
#include <cmath>

float FLOAT_EPSILON = 1e-4f;
using namespace std;

bool NotEqual(float a, float b)
{
	return !Equal(a, b);
}

bool Equal(float a, float b)
{
	return fabsf(a - b) < FLOAT_EPSILON;
}

bool LT(float a, float b)
{
	if (Equal(a, b))
		return false;
	return a < b;
}

bool LET(float a, float b)
{
	if (Equal(a, b))
		return true;
	return a < b;
}

bool MT(float a, float b)
{
	if (Equal(a, b))
		return false;
	return a > b;
}

bool MET(float a, float b)
{
	if (Equal(a, b))
		return true;
	return a > b;
}
