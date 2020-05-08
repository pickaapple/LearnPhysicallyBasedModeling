#include "Light.h"
#include "../Game/World.h"

Light::Light() :
	m_DirectionalLight({
		{1,1,1,1} , //������
		{1,1,1,1}, //������
		{1,1,1,1},//����
		{-1,0,0,0} //����
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


