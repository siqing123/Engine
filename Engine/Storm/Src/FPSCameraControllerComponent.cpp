#include "Precompiled.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "FPSCameraControllerComponent.h"

//#include "CameraService.h"
#include "GameWorld.h"
#include "TerrainService.h"

using namespace Storm;
using namespace Storm::Input;

MEMORY_POOL_DEFINE(FPSCameraControllerComponent, 1000);

void FPSCameraControllerComponent::Initialize()
{
	mCameraComponent = GetOwner().GetComponent<CameraComponent>();
}

void FPSCameraControllerComponent::Terminate()
{
	mCameraComponent = nullptr;
}

void FPSCameraControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 1.0f;
	const float turnspeed = 0.1f;

	auto& camera = mCameraComponent->GetCamera();

	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(movespeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-movespeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-movespeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(movespeed * deltaTime);

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() * turnspeed * deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() * turnspeed * deltaTime);
	}

	auto terrainService = GetOwner().GetWorld().GetService<TerrainService>();
	auto currentPosition = camera.GetPosition();
	float height = terrainService->GetHeight(currentPosition) + 2.0f;
	auto newPosition = Math::Vector3{ currentPosition.x,height,currentPosition.z };
	camera.SetPosition(newPosition);
}