#include "Mesh.h"
#include "../Game/World.h"

Mesh::Mesh(MeshData& meshData) :
	m_MeshData(meshData)
{
}

Mesh::~Mesh()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
}

const MeshData & Mesh::GetMeshData()
{
	return m_MeshData;
}

void Mesh::SetVertexPositions(const float * pos, size_t oneSize, size_t length, size_t step)
{
	m_DirtyVertexData = true;
	for (size_t i = 0; i < length; ++i)
	{
		m_MeshData.Vertex[i].Pos.SetData(pos, oneSize);
		pos += step;
	}
}

void Mesh::SetVertexPosition(const float * pos, size_t oneSize, size_t index)
{
	m_DirtyVertexData = true;
	m_MeshData.Vertex[index].Pos.SetData(pos, oneSize);
}

void Mesh::PrepareRender(ID3D11DeviceContext * context)
{
	if (m_DirtyVertexData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		context->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, m_MeshData.Vertex.data(), m_MeshData.Vertex.size() * sizeof(MeshVertex));
		context->Unmap(m_pVertexBuffer, 0);
		m_DirtyVertexData = false;
	}
}

void Mesh::Started()
{
	auto* device = World::GetInstance()->m_pD3DDevice;
	CreateVertexBuffer(device, true);
	CreateIndexBuffer(device);
}

void Mesh::CreateVertexBuffer(ID3D11Device* device, bool dynamic)
{

	// Create vertex buff
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	if (dynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = sizeof(MeshVertex) * m_MeshData.Vertex.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_MeshData.Vertex.data();
	HRESULT ret = device->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(ret))
		Debug::LogError("�������㻺��ʧ��");
	D3D11SetDebugObjectName(m_pVertexBuffer, "���񶥵㻺����");
}

void Mesh::CreateIndexBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_MeshData.Indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_MeshData.Indices.data();
	auto ret = device->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);

	if (FAILED(ret))
		Debug::LogError("������������ʧ��");

	D3D11SetDebugObjectName(m_pIndexBuffer, "��������������");
}