#include "GameObject.h"
#include "Component.h"
#include "World.h"

#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Graphics/SimpleShader.h"
#include "../Graphics/MeshRenderer.h"
#include "../Geometry/Geometry.h"


GameObject::GameObject(const string& name)
	: m_State(EGameObjectStarted)
{
	m_Transform.gameObject = this;
	auto world = World::GetInstance();
	string prefix = name.empty() ? "GameObject" : name;
	string uniqueName = prefix;
	unsigned long index = 1;
	while (world->Find(uniqueName) != nullptr)
	{
		uniqueName = prefix + "(" + to_string(index) + ")";
		index++;
	}
	m_Name = uniqueName;
	world->AddGameObject(this);
}

GameObject::~GameObject()
{
	for (Component* component : m_Components)
		delete component;
}
void GameObject::Started()
{
}
void GameObject::Start()
{
	for (Component* component : m_Components)
		component->Started();
	Started();
}
void GameObject::Update()
{
	for (Component* component : m_Components)
		component->Update();
	Updated();
}
void GameObject::Updated()
{
}
void GameObject::OnDestroy()
{
	for (Component* component : m_Components)
		component->OnDestroy();
}
Matrix GameObject::ModelMatrix()
{
	return m_Transform.GetMatrix();
}

void GameObject::Destroy()
{
	m_State = EGameObjectDestroy;
}

GameObject * GameObject::Find(const string& name)
{
	return World::GetInstance()->Find(name);
}

void GameObject::Render(ID3D11DeviceContext * context, RenderState state)
{
	for (Component* component : m_Components)
		component->Render(context, state);
}

bool GameObject::AddComponent(Component * component)
{
	if (component->gameObject) {
		Debug::LogError("组件被赋值给别的对象");
		return false;
	}
	m_Components.push_back(component);
	component->gameObject = this;
	return true;
}

GameObject* GameObject::CreateMeshObject(const string & name, MeshData & meshData)
{
	auto o = new GameObject(name);
	o->AddComponent(new Material(new SimpleShader()));
	o->AddComponent(new Mesh(meshData));
	o->AddComponent(new MeshRenderer());
	return o;
}

GameObject * GameObject::CreateCube(float width, float height, float depth,
	const Vector & color)
{
	auto&& cubeMeshData = Geometry::CreateBox(width, height, depth, color);
	auto cube = GameObject::CreateMeshObject("Cube", cubeMeshData);
	return cube;
}

GameObject* GameObject::CreateSphere(float radius, const Vector& color)
{
	auto&& meshData = Geometry::CreateSphere(radius, 16, 16, color);
	auto sphere = GameObject::CreateMeshObject("Sphere", meshData);
	return sphere;
}

GameObject * GameObject::CreateCircle(float radius, const Vector & color)
{
	auto&& meshData = Geometry::CreateCircle(radius, Vector::Zero, 16, color);
	return GameObject::CreateMeshObject("Circle", meshData);
}
