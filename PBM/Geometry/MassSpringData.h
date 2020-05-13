#pragma once
#include "../Math/Vector.h"

#include <vector>

using namespace std;

struct MeshVertex;
class Mesh;
class Particle;
class Force;
class DivForce;

enum EForceType
{
	HOOK_FORCE,
	DRAG_FORCE,
	DIV_FORCE
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
			bool PinX, PinY;
		} Hook;
		struct DivForceData
		{
			int X;
			DivForce* ForcePointer;
		}Div;
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
	//添加质点坐标，localPos为局部坐标。
	VertexData* AddVertex(const Vector& localPos, float mass);
	//添加胡克弹簧力，localA为局部坐标。
	LinkData* AddHookLink(const Vector& localA, const Vector& localB, float spring, bool pinA=false, bool pinB=false);
	//添加抓取力，localA为局部坐标。
	LinkData* AddDrag(const Vector& localA, const Vector& f);
	//添加自定义力，localA为局部坐标。
	LinkData* AddForce(const Vector&localA, DivForce* force);
	VertexData* FindVertex(const Vector& pos);
	int VertexIndex(const Vector& pos);
	VertexData* Vertex(size_t index);
	MassSpringData& operator=(MassSpringData &&other);
protected:
	vector<VertexData*> m_Vertex;
	vector<LinkData*> m_Links;
	friend class MassSpring;
};
