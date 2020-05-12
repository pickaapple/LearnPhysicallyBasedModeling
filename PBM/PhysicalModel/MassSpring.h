#pragma once
#include "Particle.h"
#include "Force.h"

#include "../Geometry/MassSpringData.h"
#include "../Game/Component.h"

class Mesh;
class MassSpringSystem;

class MassSpring : public Component
{
public:
	~MassSpring();
	void SetData(MassSpringData&& data);
	template<class _Force, class _Arg0, class _Arg1>
	_Force* AddForce(_Arg0 arg0, _Arg1 arg1);

	MassSpringSystem* GetSystem();
protected:
	virtual void Started() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
	void Clear();
public:
	//Particle中的坐标存放的是粒子全局坐标
	vector<Particle*> m_Particles;
	vector<Force*> m_Forces;
protected:
	Mesh* m_Mesh;
	MassSpringData m_Data;
	MassSpringSystem* m_MSSystem;
};

template<class _Force, class _Arg0, class _Arg1>
inline _Force * MassSpring::AddForce(_Arg0 arg0, _Arg1 arg1)
{
	auto* force = new _Force(arg0, arg1);
	m_Forces.push_back(force);
	return force;
}
