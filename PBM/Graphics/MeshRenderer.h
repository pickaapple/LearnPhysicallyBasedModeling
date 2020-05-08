#pragma once
#include "../Game/Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
protected:
	virtual void Started() override;
	virtual void Render(ID3D11DeviceContext* context, RenderState& state) override;
protected:
	Mesh* m_Mesh;
	Material* m_Material;
};

