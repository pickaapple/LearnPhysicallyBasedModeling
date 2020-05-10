#include "SunLight.h"
#include "../Graphics/Light.h"

SunLight::SunLight(const string & name):
	GameObject(name)
{
	AddComponent<Light>();
}
