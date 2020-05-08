#pragma once
#include "../Game/Component.h"
#include "../Geometry/MeshData.h"

class Mesh : public Component
{
public:
	Mesh(MeshData& vertex);
	~Mesh();
	const MeshData& GetMeshData();
	//设置顶点坐标，length表示顶点数量，oneSize表示坐标分量数，step表示坐标与坐标之间的间隔数
	void SetVertexPositions(const float* pos, size_t oneSize, size_t length, size_t step);
	void SetVertexPosition(const float* pos, size_t oneSize, size_t index);
	void PrepareRender(ID3D11DeviceContext * context);
protected:
	virtual void Started() override;
	void CreateVertexBuffer(ID3D11Device* device, bool dynamic);
	void CreateIndexBuffer(ID3D11Device* device);
protected:
	ID3D11Buffer* m_pVertexBuffer, *m_pIndexBuffer;

	MeshData m_MeshData;
	bool m_DirtyVertexData;
	friend class MeshRenderer;
};

