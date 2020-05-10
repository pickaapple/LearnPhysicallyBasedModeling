#include "Shader.h"
#include "../Common/ShaderException.h"

Shader::~Shader()
{
	if (m_VSConstantBuffer) m_VSConstantBuffer->Release();
	if (m_PSConstantBuffer) m_PSConstantBuffer->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
}

void Shader::PrepareRender(ID3D11DeviceContext * context, Material* material)
{
	context->IASetInputLayout(m_pVertexLayout);
	context->VSSetShader(m_pVertexShader, NULL, 0);
	context->PSSetShader(m_pPixelShader, NULL, 0);
}

void Shader::CreateConstantBuffer(ID3D11Device* device, UINT byteWith, ID3D11Buffer ** buffer)
{
	//创建常量缓存
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = byteWith;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	auto ret = device->CreateBuffer(&bd, NULL, buffer);
	if (FAILED(ret))
		throw new ShaderException("创建常量缓存失败");
}

void Shader::CompileShaderFromFile(ID3D10Blob* &blob, LPCWSTR filename, LPCSTR entry, LPCSTR target)
{
	UINT flag1 = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flag1 |= D3DCOMPILE_DEBUG; //开启Debug模式
	flag1 |= D3DCOMPILE_SKIP_OPTIMIZATION; // 禁止优化
#endif
	ID3D10Blob *p_errorMessage = nullptr;
	auto ret = D3DCompileFromFile((m_RootPath + filename).data(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, target, flag1, NULL, &blob, &p_errorMessage);
	if (FAILED(ret)) {
		string errorInfo("着色器编译发生错误");
		if (p_errorMessage) {
			char * message = (char *)p_errorMessage->GetBufferPointer();
			SIZE_T size = p_errorMessage->GetBufferSize();
			string error(message, size);
			errorInfo += '\n';
			errorInfo += error;
		}
		throw new ShaderException(errorInfo.data());
	}
	if (p_errorMessage)
		p_errorMessage->Release();
}
