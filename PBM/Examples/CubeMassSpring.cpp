#include "CubeMassSpring.h"

#include "../Game/World.h"
#include "../Game/GameObject.h"
#include "../Geometry/Geometry.h"
#include "../PhysicalModel/MassSpring.h"
#include "../PhysicalModel/MassSpringSystem.h"

CubeMassSpring::CubeMassSpring()
{
	auto* world = World::GetInstance();
	auto cube = GameObject::CreateCube(2, 2, 2, Vector(1, 1, 1, 1));
	cube->m_Transform.Move(0, 4, 0);
	auto massSpring = cube->AddComponent<MassSpring>();
	auto cubeMSData = Geometry::CreateBoxMS(2, 2, 2, 1, 1);
	cubeMSData.AddDrag(cubeMSData.Vertex(0)->m_Pos, Vector({ 0,-2,0,0 }));
	massSpring->SetData(move(cubeMSData));
	//关闭重力
	massSpring->GetSystem()->SetGravityValid(false);

	auto sphere = GameObject::CreateSphere(3, Vector(1, 1, 1, 1));
	sphere->m_Transform.Move(4, 4, 0);
	world->m_pMainCamera->SetCameraMode(THIRD_PLAYER, sphere);
}
