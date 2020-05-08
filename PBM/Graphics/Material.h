#pragma once
#include "../Game/Component.h"
#include "Shader.h"

struct MaterialData
{
	Vector Ambient;
	Vector Diffuse;
	Vector Specular;
	Vector Reflect;
};

class Material : public Component
{
public:
	Material(Shader* shader);
public:
	MaterialData m_MaterialData;
protected:
	Shader* m_Shader;
	
	friend class MeshRenderer;
};


