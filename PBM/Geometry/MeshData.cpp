#include "MeshData.h"

void VertexBundle::SetPos(const Vector & pos)
{
	for (auto v : m_Bundle)
		v->Pos = pos;
}
