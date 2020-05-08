#pragma once
#include "GameObject.h"
#include "Camera.h"

class Keyboard;
class Mouse;
class Light;

class World
{
public:
	static World* GetInstance();
	void InitResource();
	bool Run();
	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	GameObject* Find(const string& name);
protected:
	World();
	~World();
public:
	double m_DeltaTime;  // 上一帧所花的时间 单位是ms
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;

	Keyboard* m_pKeyboard;
	Mouse* m_pMouse;
	Camera* m_pMainCamera;
	vector<Light*> m_Lights;
protected:
	map<string, GameObject*> m_GameObjectMap;
	vector<GameObject*> m_GameObjectList;
};
