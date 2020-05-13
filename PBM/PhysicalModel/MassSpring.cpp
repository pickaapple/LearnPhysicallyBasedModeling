#include "MassSpring.h"
#include "../Game/GameObject.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Mesh.h"
#include "MassSpringSystem.h"
#include "HookForce.h"
#include "DragForce.h"
#include "DivForce.h"

MassSpring::~MassSpring()
{
	Clear();
}

void MassSpring::Clear()
{
	for (size_t i = 0; i < m_Particles.size(); ++i)delete m_Particles[i];
	for (size_t i = 0; i < m_Forces.size(); ++i)delete m_Forces[i];
	m_Particles.clear();
	m_Forces.clear();
}

MassSpringSystem * MassSpring::GetSystem()
{
	if (m_MSSystem == nullptr) {
		m_MSSystem = GameObject::Find<MassSpringSystem>("MassSpringSystem");
		if (m_MSSystem == nullptr)
			m_MSSystem = new MassSpringSystem();
	}
	return m_MSSystem;
}

void MassSpring::SetData(MassSpringData&& data)
{
	m_Data = std::move(data);
}

void MassSpring::Started()
{
	m_Mesh = gameObject->GetComponent<Mesh>();
	if (m_Mesh == nullptr)
	{
		Debug::LogError("MassSpring组件所在的GameObject缺少Mesh组件");
		return;
	}

	auto matrix = gameObject->m_Transform.GetMatrix();

	for (auto v : m_Data.m_Vertex)
		m_Particles.push_back(new Particle(v->m_Pos * matrix, v->m_Mass));
	for (auto& l : m_Data.m_Links) {
		Force* force = nullptr;
		switch (l->Type)
		{
		case HOOK_FORCE:
		{
			auto& data = l->Data.Hook;
			auto hook = new HookForce(m_Particles[data.X], m_Particles[data.Y], data.Spring);
			hook->m_PinA = data.PinX;
			hook->m_PinB = data.PinY;
			force = hook;
		}
		break;
		case DRAG_FORCE:
		{
			auto& data = l->Data.Drag;
			force = new DragForce(m_Particles[data.X], data.Force);
		}
		break;
		case DIV_FORCE:
		{
			auto& data = l->Data.Div;
			data.ForcePointer->m_P = m_Particles[data.X];
			force = data.ForcePointer;
		}
		break;
		default:
			continue;
		}
		m_Forces.push_back(force);
	}
	auto system = GetSystem();
	system->AddMassSpring(this);
}

void MassSpring::Update()
{
	if (m_Mesh)
	{
		for (size_t i = 0; i < m_Particles.size(); ++i)
		{
			m_Data.m_Vertex[i]->Update(*m_Mesh, *m_Particles[i]);
		}
	}
}

void MassSpring::OnDestroy()
{
	m_MSSystem->RemoveMassSpring(this);
}
