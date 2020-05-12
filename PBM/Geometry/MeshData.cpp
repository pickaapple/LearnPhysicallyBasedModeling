#include "MeshData.h"
#include <utility>

void VertexBundle::SetPos(const Vector & pos)
{
	for (auto v : m_Bundle)
		v->Pos = pos;
}

vector<unsigned short> MeshData::GetIndices()
{
	vector<unsigned short> ans(TriangleIndices.size() + LineIndices.size());
	size_t i = 0;
	for (size_t j = 0; j < TriangleIndices.size(); ++j) ans[i++] = TriangleIndices[j];
	for (size_t j = 0; j < LineIndices.size(); ++j) ans[i++] = LineIndices[j];
	return std::move(ans);
}
