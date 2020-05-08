#pragma once
#include "Shader.h"

class SimpleShader : public Shader
{
public:
	SimpleShader();
	virtual void PrepareRender(ID3D11DeviceContext * context, Material* material) override;
};

