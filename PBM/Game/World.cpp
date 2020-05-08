#include "../stdafx.h"
#include "GameObject.h"
#include "World.h"
#include "SunLight.h"

#include "../PhysicalModel/MassSpring.h"

#include "../Windows/Mouse.h"
#include "../Windows/Keyboard.h"

World::World() :m_DeltaTime(1.0) {}

World::~World()
{
	for (auto gameObject : m_GameObjectList)
		delete gameObject;
}

void World::InitResource()
{
	// 设置主摄像机
	m_pMainCamera = new Camera("MainCamera");
	m_pKeyboard->RegisterKeyStateListener(KEY_R, KEY_STATE_DOWN, m_pMainCamera);

	auto cube = CreateCube(2, 2, 2, Vector(1, 1, 1, 1));
	cube->AddComponent<MassSpring>();

	auto sphere = CreateSphere(3, Vector(1, 1, 1, 1));
	sphere->m_Transform.Move(4, 0, 0);
	m_pMainCamera->SetCameraMode(THIRD_PLAYER, sphere);
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
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

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
		m_GameObjectList.erase(remove(m_GameObjectList.begin(), m_GameObjectList.end(), gameObject), m_GameObjectList.end());
		m_GameObjectMap.erase(itr);
	}
}

