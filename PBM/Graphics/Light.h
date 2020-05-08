#pragma once
#include "../Game/Component.h"

struct DirectionalLight
{
	Vector Ambient;
	Vector Diffuse;
	Vector Specular;
	Vector Direction;
};

class Light : public Component
{
public:
	Light();
	~Light();
public:
	DirectionalLight m_DirectionalLight;
};
