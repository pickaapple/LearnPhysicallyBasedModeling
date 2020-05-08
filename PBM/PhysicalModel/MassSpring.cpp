#include "MassSpring.h"
#include "../Game/GameObject.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Mesh.h"
#include "MassSpringSystem.h"
#include "HookForce.h"

MassSpring::~MassSpring()
{
	for (size_t i = 0; i < m_Particles.size(); ++i)delete m_Particles[i];
	for (size_t i = 0; i < m_Forces.size(); ++i)delete m_Forces[i];
}

void MassSpring::Started()
{
	m_Mesh = gameObject->GetComponent<Mesh>();
	if (m_Mesh == nullptr)
	{
		Debug::LogError("MassSpring组件所在的GameObject缺少Mesh组件");
		return;
	}
	m_MSSystem = GameObject::Find<MassSpringSystem>("MassSpringSystem");
	if (m_MSSystem == nullptr)
		m_MSSystem = new MassSpringSystem();
	const auto& meshData = m_Mesh->GetMeshData();
	size_t vertexCount = meshData.Vertex.size();
	for (size_t i = 0; i < vertexCount; ++i)
	{
		auto& vertex = meshData.Vertex[i];
		auto particle = new Particle(vertex.Pos.data(), 1);
		m_Particles.push_back(particle);
	}
	size_t triangleCount = meshData.Indices.size() / 3;
	vector<vector<bool>> connectFlags(vertexCount, vector<bool>(vertexCount, false));
	auto pushForce = [&connectFlags, this](int index1, int index2)
	{
		if (!connectFlags[index1][index2])
		{
			connectFlags[index1][index2] = true;
			connectFlags[index2][index1] = true;
			auto force = new HookForce(m_Particles[index1], m_Particles[index2]);
			m_Forces.push_back(force);
		}
	};
	for (size_t i = 0; i < triangleCount; ++i)
	{
		size_t start = i * 3;
		auto index1 = meshData.Indices[start];
		auto index2 = meshData.Indices[start + 1];
		auto index3 = meshData.Indices[start + 2];
		pushForce(index1, index2);
		pushForce(index1, index3);
		pushForce(index2, index3);
	}
	m_MSSystem->AddMassSpring(this);
}

void MassSpring::Update()
{
	if (m_Mesh)
	{
		for (size_t i = 0; i < m_Particles.size(); ++i)
		{
			auto* p = m_Particles[i];
			m_Mesh->SetVertexPosition(p->GetPos(), 3, i);
		}
	}
}

void MassSpring::OnDestroy()
{
	m_MSSystem->RemoveMassSpring(this);
}
