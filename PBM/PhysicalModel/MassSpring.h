#pragma once
#include "../Game/Component.h"
#include "Particle.h"
#include "Force.h"

class Mesh;
class MassSpringSystem;

class MassSpring : public Component
{
public:
	~MassSpring();
protected:
	virtual void Started() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
public:
	vector<Particle*> m_Particles;
	vector<Force*> m_Forces;
protected:
	Mesh* m_Mesh;
	MassSpringSystem* m_MSSystem;
};

