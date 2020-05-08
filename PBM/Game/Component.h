#pragma once
#include "../stdafx.h"
class GameObject;
class Component
{
protected:
	Component();
	virtual void Render(ID3D11DeviceContext* context, RenderState& state);
	virtual void Started();
	virtual void Update();
	virtual void OnDestroy();
public:
	GameObject* gameObject;
	friend class GameObject;
};

