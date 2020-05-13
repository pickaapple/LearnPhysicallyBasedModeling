#include "MassSpringData.h"
#include "MeshData.h"
#include "../Graphics/Mesh.h"
#include "../PhysicalModel/Particle.h"
#include "../Game/GameObject.h"
#include "../Game/Transform.h"


VertexData::VertexData(const Vector & pos, float mass, vector<size_t> indices) :
	m_Pos(pos), m_Mass(mass), m_MeshVertexIndex(indices)
{
}

void VertexData::Update(Mesh & mesh, const Particle & particle)
{
	auto matrix = mesh.gameObject->m_Transform.GetMatrix().Inversed();
	auto particlePos = particle.GetPos()*matrix;
	auto delta = particlePos - m_Pos;
	for (auto i : m_MeshVertexIndex)
		mesh.AddVertexPosition(delta, i);
	m_Pos = particlePos;
}

MassSpringData::MassSpringData()
{
}

MassSpringData::MassSpringData(MassSpringData && other)
{
	for (auto * p : other.m_Vertex) m_Vertex.push_back(p);
	for (auto * p : other.m_Links) m_Links.push_back(p);
	other.m_Vertex.clear();
	other.m_Links.clear();
}

MassSpringData::~MassSpringData()
{
	for (auto * p : m_Vertex) delete p;
	for (auto * p : m_Links) delete p;
	m_Vertex.clear();
	m_Links.clear();
}

VertexData * MassSpringData::AddVertex(const Vector & localPos, float mass)
{
	auto curData = new VertexData(localPos, mass, {});
	m_Vertex.push_back(curData);
	return curData;
}

LinkData * MassSpringData::AddHookLink(const Vector & localA, const Vector & localB, float spring, bool pinA, bool pinB)
{
	int ia = VertexIndex(localA);
	if (ia < 0) return nullptr;
	int ib = VertexIndex(localB);
	if (ib < 0) return nullptr;
	auto* link = new LinkData({});
	link->Type = HOOK_FORCE;
	link->Data.Hook = { ia, ib, spring ,pinA ,pinB };
	m_Links.push_back(link);
	return link;
}

LinkData * MassSpringData::AddDrag(const Vector & localA, const Vector & f)
{
	int ia = VertexIndex(localA);
	if (ia < 0) return nullptr;
	auto* link = new LinkData();
	link->Type = DRAG_FORCE;
	link->Data.Drag = { ia, f };
	m_Links.push_back(link);
	return link;
}

LinkData * MassSpringData::AddForce(const Vector & localA, DivForce * force)
{
	int ia = VertexIndex(localA);
	if (ia < 0) return nullptr;
	auto* link = new LinkData();
	link->Type = DIV_FORCE;
	link->Data.Div = { ia, force };
	m_Links.push_back(link);
	return link;
}

VertexData * MassSpringData::FindVertex(const Vector & pos)
{
	for (auto* d : m_Vertex)
	{
		if (d->m_Pos.Equals(pos))
			return d;
	}
	return nullptr;
}

int MassSpringData::VertexIndex(const Vector & pos)
{
	for (int i = 0; i < m_Vertex.size(); ++i)
	{
		auto* d = m_Vertex[i];
		if (d->m_Pos.Equals(pos))
			return i;
	}
	return -1;
}

VertexData * MassSpringData::Vertex(size_t index)
{
	if (index >= m_Vertex.size())
		return nullptr;
	return m_Vertex[index];
}

MassSpringData & MassSpringData::operator=(MassSpringData && other)
{
	for (auto * p : other.m_Vertex) m_Vertex.push_back(p);
	for (auto * p : other.m_Links) m_Links.push_back(p);
	other.m_Vertex.clear();
	other.m_Links.clear();
	return *this;
}

LinkData::LinkData() :
	Data({ 0 })
{
}
