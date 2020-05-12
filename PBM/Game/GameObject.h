#pragma once
#include "Transform.h"

class Component;
class MeshData;

enum EState {
	EGameObjectStarted,
	EGameObjectUpdated,
	EGameObjectDestroy
};

class GameObject
{
public:
	GameObject(const string& name = "");
	virtual ~GameObject();

	template <class _Comp>
	inline _Comp* AddComponent();
	template <class _Comp, class _A1>
	inline _Comp* AddComponent(_A1 a1);
	bool AddComponent(Component* component);
	template<class _Comp>
	_Comp* GetComponent();

	static GameObject* Find(const string& name);
	template<class T>
	static T* Find(const string& name);

	Matrix ModelMatrix();
	void Destroy();

	static GameObject * CreateMeshObject(const string & name, MeshData & meshData);
	static GameObject* CreateCube(float width, float height, float depth, const Vector& color);
	static GameObject* CreateSphere(float radius, const Vector& color);
protected:
	void Start();
	void Update();
	virtual void Render(ID3D11DeviceContext* context, RenderState state);
	virtual void Started();
	virtual void Updated();
	virtual void OnDestroy();
public:
	Transform m_Transform;
	bool m_RelativeCoordinate = true; // 使用全局坐标系 还是 相对坐标系
	string m_Name;
protected:
	EState m_State;
	vector<Component*> m_Components;
private:
	friend class World;
	friend class GameObjectGroup;
};

template<class _Comp>
_Comp* GameObject::AddComponent()
{
	auto* component = new _Comp();
	static_assert(is_base_of<Component, _Comp>::value, "组件必须继承于Component类");
	if (AddComponent(component))
		return component;
	return nullptr;
}

template<class _Comp, class _A1>
_Comp* GameObject::AddComponent(_A1 a1)
{
	return AddComponent(new _Comp(a1));
}

template<class _Comp>
inline _Comp * GameObject::GetComponent()
{
	for (auto component : m_Components) {
		auto ans = dynamic_cast<_Comp*>(component);
		if (ans)
			return ans;
	}
	return nullptr;
}

template<class T>
T* GameObject::Find(const string& name)
{
	auto gameObject = GameObject::Find(name);
	return dynamic_cast<T*>(gameObject);
}
