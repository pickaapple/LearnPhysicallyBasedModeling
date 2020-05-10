#include "Light.h"
#include "../Game/World.h"

Light::Light() :
	m_DirectionalLight({
		{1,1,1,1} , //环境光
		{1,1,1,1}, //漫反射
		{1,1,1,1},//镜面
		{-1,0,0,0} //方向
		})
{
	auto world = World::GetInstance();
	world->m_Lights.push_back(this);
}

Light::~Light()
{
	auto world = World::GetInstance();
	auto& lights = world->m_Lights;
	lights.erase(remove(lights.begin(), lights.end(), this), lights.end());
}


