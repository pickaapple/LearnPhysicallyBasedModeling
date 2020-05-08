#pragma once
#include "../stdafx.h"

class Material;

class Shader
{
public:
	~Shader();
	virtual void PrepareRender(ID3D11DeviceContext * context, Material* material);
protected:
	void CreateConstantBuffer(ID3D11Device* device, UINT byteWith, ID3D11Buffer** buffer);
	void CompileShaderFromFile(ID3D10Blob* &blob, LPCWSTR filename, LPCSTR entry, LPCSTR target);
public:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;
	ID3D11Buffer* m_VSConstantBuffer, *m_PSConstantBuffer;
	ID3D11InputLayout *m_pVertexLayout;
protected:
	const wstring m_RootPath = L"./Graphics/Shaders/";
};

