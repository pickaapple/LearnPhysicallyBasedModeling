#include "../stdafx.h"
#include "GameObject.h"
#include "World.h"
#include "SunLight.h"

#include "../Common/utility.h"
#include "../Geometry/Geometry.h"
#include "../PhysicalModel/MassSpring.h"
#include "../PhysicalModel/MassSpringSystem.h"
#include "../PhysicalModel/DragForce.h"

#include "../Windows/Mouse.h"
#include "../Windows/Keyboard.h"

#include "../Examples/CubeMassSpring.h"
#include "../Examples/ABeadOnAWire.h"

World::World() :m_DeltaTime(1.0) {}

World::~World()
{
	for (auto gameObject : m_GameObjectList)
		delete gameObject;
}

void CreateCoordinate()
{
	MeshData meshData;
	meshData.Vertex.push_back({ {0,0,0,1} , Color::Red });
	meshData.Vertex.push_back({ {1,0,0,1} , Color::Red });
	meshData.Vertex.push_back({ {0,0,0,1} , Color::Green });
	meshData.Vertex.push_back({ {0,1,0,1} , Color::Green });
	meshData.Vertex.push_back({ {0,0,0,1} , Color::Blue });
	meshData.Vertex.push_back({ {0,0,1,1} , Color::Blue });
	meshData.LineIndices.push_back(0);
	meshData.LineIndices.push_back(1);
	meshData.LineIndices.push_back(2);
	meshData.LineIndices.push_back(3);
	meshData.LineIndices.push_back(4);
	meshData.LineIndices.push_back(5);
	GameObject::CreateMeshObject("Coordinate", meshData);
}

void World::InitResource()
{
	// 设置主摄像机
	m_pMainCamera = new Camera("MainCamera");
	m_pKeyboard->RegisterKeyStateListener(KEY_R, KEY_STATE_DOWN, m_pMainCamera);

	//CubeMassSpring example1;
	ABeadOnAWire example2;

	//CreateCoordinate();
	auto groundGridData = Geometry::CreateGrid(50, (unsigned short)25, Color::Gray);
	GameObject::CreateMeshObject("GroundGrid", groundGridData);

	auto light = new SunLight("Light0");
	m_pMouse->BeginTracking();
}

World * World::GetInstance()
{
	static World* world = new World();
	return world;
}

bool World::Run()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	for (size_t i = 0; i < m_GameObjectList.size(); ++i)
	{
		auto* gameObject = m_GameObjectList[i];
		switch (gameObject->m_State) {
		case EGameObjectStarted:
			gameObject->m_State = EGameObjectUpdated;
			gameObject->Start();
			break;
		case EGameObjectUpdated:
			gameObject->Update();
			gameObject->Render(m_pImmediateContext, { Matrix::Identity });
			break;
		case EGameObjectDestroy:
			gameObject->OnDestroy();
			RemoveGameObject(gameObject);
			--i;
			break;
		}
	}
	m_pSwapChain->Present(0, 0);
	return true;
}

GameObject* World::Find(const string& name)
{
	auto itr = m_GameObjectMap.find(name);
	if (itr != m_GameObjectMap.end())
		return (*itr).second;
	return nullptr;
}

void World::AddGameObject(GameObject * gameObject)
{
	auto name = gameObject->m_Name;
	auto itr = m_GameObjectMap.find(name);
	if (itr == m_GameObjectMap.end()) {
		m_GameObjectList.push_back(gameObject);
		m_GameObjectMap.insert(make_pair(name, gameObject));
	}
	else {
		Debug::LogError("游戏对象'" + name + "'已存在");
	}
}

void World::RemoveGameObject(GameObject * gameObject)
{
	auto name = gameObject->m_Name;
	auto itr = m_GameObjectMap.find(name);
	if (itr != m_GameObjectMap.end()) {
		ErazeRemove(m_GameObjectList, gameObject);
		m_GameObjectMap.erase(itr);
	}
}

