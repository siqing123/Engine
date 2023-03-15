#include "Precompiled.h"
#include "PhysicsComponent.h"
#include "GameWorld.h"
#include"RigidBodyComponent.h"
#include "TransformComponent.h"

using namespace Storm;

void PhysicsComponent::Initialize()
{	 
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
}	 
	 
void PhysicsComponent::Terminate()
{	 
	mTransformComponent = nullptr;
}

void Storm::PhysicsComponent::UpdatePhysics()
{
	if (mRigidBodyComponent != nullptr)
	{
		auto& rigidBodyTransform = mRigidBodyComponent->GetRigidBody()->getWorldTransform();
		ConverToTransform(rigidBodyTransform, *mTransformComponent);
	}
}
	  
void PhysicsComponent::DebugUI()
{

}
