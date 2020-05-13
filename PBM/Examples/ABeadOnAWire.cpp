#include "ABeadOnAWire.h"

#include "../Geometry/MassSpringData.h"
#include "../Graphics/Material.h"
#include "../Graphics/Mesh.h"
#include "../Game/World.h"
#include "../Game/GameObject.h"
#include "../PhysicalModel/DivForce.h"
#include "../PhysicalModel/Particle.h"
#include "../PhysicalModel/MassSpring.h"
#include "../PhysicalModel/MassSpringSystem.h"

class BeadForce : public DivForce
{
public:
	BeadForce(const Vector& center);
	virtual void ApplyForce(ParticleSystem& system, float deltaTime) override;
protected:
	Vector m_Center;
};

void BeadForce::ApplyForce(ParticleSystem& system, float deltaTime)
{
	float magicAlpha = 0.008f;
	float magicBeta = 0.04f;
	auto pos = m_P->GetPos();
	auto x = pos - m_Center;
	auto m = m_P->GetMass();
	auto f = m_P->GetForce();
	auto v = m_P->GetVelocity();
	auto C = (Vector::Dot(x, x) - 1) / 2;
	auto Cdot = Vector::Dot(x, v);
	float lamdba = (-Vector::Dot(f, x) - m * Vector::Dot(v, v) /*- m * magicAlpha * C - m * magicBeta * Cdot*/) / Vector::Dot(x, x);
	auto constraintedForce = lamdba * x;

	m_P->AddForce(constraintedForce);
}

BeadForce::BeadForce(const Vector& center) :
	m_Center(center)
{
}


ABeadOnAWire::ABeadOnAWire()
{
	auto* world = World::GetInstance();
	auto wire = GameObject::CreateCircle(2, Color::Red);
	auto material = wire->GetComponent<Material>();
	material->m_MaterialData.Ambient = Vector(1, 1, 1, 1);
	material->m_MaterialData.Diffuse = Vector(1, 1, 1, 1);
	wire->m_Transform.Move({ 0,4,0 });

	auto bead = GameObject::CreateSphere(0.3, Color::Green);
	bead->m_Transform.Move({ 2,4,0 });
	auto beadMesh = bead->GetComponent<Mesh>();
	auto beadMS = bead->AddComponent<MassSpring>();

	MassSpringData beadMSData;
	auto vertex = beadMSData.AddVertex({ 0,0,0,1 }, 1);
	for (size_t i = 0; i < beadMesh->GetMeshData().Vertex.size(); ++i)
		vertex->m_MeshVertexIndex.push_back(i);
	beadMSData.AddVertex({ -2,0,0,1 }, 1);
	beadMSData.AddDrag({ 0,0,0,1 }, { -1,0,-1,0 });
	beadMSData.AddForce({ 0,0,0,1 }, new BeadForce({ 0,4,0,1 }));
	beadMS->SetData(move(beadMSData));
	beadMS->GetSystem()->SetGravityValid(false);
	world->m_pMainCamera->SetCameraMode(THIRD_PLAYER, bead);
}
