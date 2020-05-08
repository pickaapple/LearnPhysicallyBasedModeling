#include "../Common/ShaderException.h"
#include "../Game/World.h"
#include "SimpleShader.h"
#include "Material.h"
#include "Light.h"

struct VSConstantBuffer
{
	XMMATRIX World, View, Projection, WorldInvTranspose;
};

struct PSConstantBuffer
{
	DirectionalLight DirLight;
	MaterialData Material;
	Vector EyePosW;
};

SimpleShader::SimpleShader()
{
	auto vs_filepath = L"Simple_VS.hlsl";
	auto ps_filepath = L"Simple_PS.hlsl";
	//创建Shader
	ID3D10Blob *p_vsBlob = nullptr, *p_psBlob = nullptr;
	auto_release<ID3D10Blob> a_vsBlob(p_vsBlob), a_psBlob(p_psBlob);

	CompileShaderFromFile(p_vsBlob, vs_filepath, "VSMain", "vs_4_0");
	CompileShaderFromFile(p_psBlob, ps_filepath, "PSMain", "ps_4_0");

	ID3D11Device* device = World::GetInstance()->m_pD3DDevice;
	auto ret = device->CreateVertexShader(p_vsBlob->GetBufferPointer(), p_vsBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(ret))
		throw new ShaderException("创建顶点着色器失败");
	ret = device->CreatePixelShader(p_psBlob->GetBufferPointer(), p_psBlob->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(ret))
		throw new ShaderException("创建像素着色器失败");

	CreateConstantBuffer(device, sizeof(VSConstantBuffer), &m_VSConstantBuffer);
	CreateConstantBuffer(device, sizeof(PSConstantBuffer), &m_PSConstantBuffer);

	//输入布局
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	UINT numElements = ARRAYSIZE(layout);
	ret = device->CreateInputLayout(layout, numElements,
		p_vsBlob->GetBufferPointer(), p_vsBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(ret))
		throw new ShaderException("创建创建输入布局失败失败");

	D3D11SetDebugObjectName(m_pVertexShader, "SimpleVS");
	D3D11SetDebugObjectName(m_pPixelShader, "SimplePS");
	D3D11SetDebugObjectName(m_pVertexLayout, "SimpleIL");
	D3D11SetDebugObjectName(m_VSConstantBuffer, "SimpleVSCB");
	D3D11SetDebugObjectName(m_PSConstantBuffer, "SimplePSCB");
}

void SimpleShader::PrepareRender(ID3D11DeviceContext * context, Material* material)
{
	Shader::PrepareRender(context, material);
	context->VSSetConstantBuffers(0, 1, &m_VSConstantBuffer);
	context->PSSetConstantBuffers(1, 1, &m_PSConstantBuffer);

	auto* world = World::GetInstance();
	auto model = material->gameObject->ModelMatrix();
	auto camera = world->m_pMainCamera;
	auto view = camera->ViewMatrix();
	auto projection = camera->ProjectionMatrix();

	VSConstantBuffer cb;
	cb.World = model.Transposed().m_M;
	cb.View = view.Transposed().m_M;
	cb.Projection = projection.Transposed().m_M;
	cb.WorldInvTranspose = model.Inversed().m_M;
	context->UpdateSubresource(m_VSConstantBuffer, 0, NULL, &cb, 0, 0);

	if (!world->m_Lights.empty())
	{
		PSConstantBuffer pscb;
		pscb.DirLight = world->m_Lights[0]->m_DirectionalLight;
		pscb.Material = material->m_MaterialData;
		pscb.EyePosW = camera->m_Transform.GetPosition();
		context->UpdateSubresource(m_PSConstantBuffer, 0, NULL, &pscb, 0, 0);
	}
}
