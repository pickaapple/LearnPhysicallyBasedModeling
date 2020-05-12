#pragma once
#include "../Math/Vector.h"

#include <vector>

using namespace std;

struct MeshVertex;
class Mesh;
class Particle;
class Force;

enum EForceType
{
	HOOK_FORCE,
	DRAG_FORCE
};

struct LinkData
{
	LinkData();
	EForceType Type;
	typedef union {
		struct DragForceData
		{
			int X;
			Vector Force;
		} Drag;
		struct HookForceData
		{
			int X, Y;
			float Spring;
		} Hook;
	} ForceData;
	ForceData Data;
};

//绑定质点与Mesh顶点的类
class VertexData
{
public:
	VertexData(const Vector& pos, float mass, vector<size_t> indices);
	virtual void Update(Mesh& mesh, const Particle& particle);
	Vector m_Pos;
	float m_Mass;
	vector<size_t> m_MeshVertexIndex; // 对应Mesh顶点索引
};

class MassSpringData
{
public:
	MassSpringData();
	MassSpringData(MassSpringData&& other);
	~MassSpringData();
	VertexData* AddVertex(const Vector& pos, float mass);
	LinkData* AddHookLink(const Vector& a, const Vector& b, float spring);
	LinkData* AddDrag(const Vector& a, const Vector& f);
	VertexData* FindVertex(const Vector& pos);
	int VertexIndex(const Vector& pos);
	VertexData* Vertex(size_t index);
	MassSpringData& operator=(MassSpringData &&other);
protected:
	vector<VertexData*> m_Vertex;
	vector<LinkData*> m_Links;
	friend class MassSpring;
};
