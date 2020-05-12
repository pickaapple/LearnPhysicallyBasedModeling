#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"

#include "../Game/GameObject.h"
#include "../Game/World.h"
#include "../Windows/RenderSetting.h"

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::Started()
{
	m_Mesh = gameObject->GetComponent<Mesh>();
	if (!m_Mesh)
		Debug::LogError("Mesh为空");
	m_Material = gameObject->GetComponent<Material>();
	if (!m_Material)
		Debug::LogError("Material为空");
}

void MeshRenderer::Render(ID3D11DeviceContext * context, RenderState& state)
{
	m_Mesh->PrepareRender(context);

	m_Material->m_Shader->PrepareRender(context, m_Material);

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_Mesh->m_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_Mesh->m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	UINT start = 0;
	UINT count = (UINT)(m_Mesh->m_MeshData.TriangleIndices.size());
	if (count != 0)
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(count, start, 0);
		start += count;
	}
	count = (UINT)(m_Mesh->m_MeshData.LineIndices.size());
	if (count != 0)
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->DrawIndexed(count, start, 0);
		start += count;
	}
}
