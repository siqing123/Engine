#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include"GameWorld.h"

using namespace Storm;

MEMORY_POOL_DEFINE(ColliderComponent, 1000);

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCollisionShape = new btBoxShape(btVector3(mExtend.x, mExtend.y, mExtend.z));
}

void ColliderComponent::DebugUI()
{
	const auto& aabb = GetAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, Graphics::Colors::Red);
}

void ColliderComponent::Terminate()
{
	mTransformComponent = nullptr;
	//SafeReleasePhysics(mCollisionShape);
}

Math::AABB ColliderComponent::GetAABB() const
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->position + mCenter;
	aabb.extend = mExtend;
	return aabb;
}
