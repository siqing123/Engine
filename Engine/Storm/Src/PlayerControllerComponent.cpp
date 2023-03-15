#include "Precompiled.h"
#include "PlayerControllerComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "TerrainService.h"

using namespace Storm;
using namespace Storm::Input;
using namespace Storm::Math;

MEMORY_POOL_DEFINE(PlayerControllerComponent, 1000)

void PlayerControllerComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mAnimatorComponent = GetOwner().GetComponent<AnimatorComponent>();
}

void PlayerControllerComponent::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float moveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? mRunSpeed : mWalkSpeed;
	const float turnSpeed = 0.1f;

	Matrix4 transform = mTransformComponent->GetMatrix4();
	Vector3 velocity = Vector3::Zero;

	if (inputSystem->IsKeyDown(KeyCode::W))
		velocity = GetLook(transform) * moveSpeed;
	if (inputSystem->IsKeyDown(KeyCode::S))
		velocity = GetLook(transform) * -moveSpeed;
	if (inputSystem->IsKeyDown(KeyCode::D))
		velocity = GetRight(transform) * moveSpeed;
	if (inputSystem->IsKeyDown(KeyCode::A))
		velocity = GetRight(transform) * -moveSpeed;

	Vector3 position = GetTranslation(transform);
	position += velocity * deltaTime;

	//if (MagnitudeSqr(velocity) > 0.0f)
	//	mAnimatorComponent->PlayAnimation(AnimState::Walk);
	//else
	//	mAnimatorComponent->PlayAnimation(AnimState::Idle);


	 //Snap to terrain
	auto terrainService = GetOwner().GetWorld().GetService<TerrainService>();
	float height = terrainService->GetHeight(position);
	auto newPosition = Math::Vector3{ position.x, height, position.z };
	mTransformComponent->position = newPosition;

	// Rotation
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		auto mouseX = inputSystem->GetMouseMoveX() * deltaTime;
		auto rotation = Quaternion::RotationEuler({ 0.0f, mouseX, 0.0f });
		mTransformComponent->rotation *= rotation;
	}

	//Update animation
	if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
		mAnimatorComponent->PlayAnimation(AnimState::Run);
	else if (MagnitudeSqr(velocity) > 150.0f)
		mAnimatorComponent->PlayAnimation(AnimState::Run);
	else if (MagnitudeSqr(velocity) > 0.0f)
		mAnimatorComponent->PlayAnimation(AnimState::Walk);
	else //if (inputSystem->IsMousePressed(MouseButton::RBUTTON))
		mAnimatorComponent->PlayAnimation(AnimState::Idle);

}

void PlayerControllerComponent::DebugUI()
{

}
