#include"Precompiled.h"
#include "RigidBodyComponent.h"
#include"ColliderComponent.h"
#include"TransformComponent.h"
#include"GameObject.h"




using namespace Storm;
using namespace Storm::Graphics;

void RigidBodyComponent::Initialize()
{
	auto colliderComponent = GetOwner().GetComponent<ColliderComponent>();
	auto transformComponent = GetOwner().GetComponent<TransformComponent>();

	auto collisionShap = colliderComponent->GetCollisionShap();
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (IsDynamic())
	{
		collisionShap->calculateLocalInertia(mMass, localInertia);
	}

	btDefaultMotionState* motionState = new btDefaultMotionState(ConvertTobtTransform(*transformComponent));
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mMass,motionState, collisionShap,localInertia);
	mRigidBody = new btRigidBody(rbInfo);
}

void  RigidBodyComponent::Terminate()
{
	if (mRigidBody->getMotionState())
	{
		delete mRigidBody->getMotionState();
	}

	SafeReleasePhysics(mRigidBody);
}

void  RigidBodyComponent::SetMass(float mass)
{
	mMass = mass;
}

bool RigidBodyComponent::IsDynamic() const
{
	return mMass > 0.0f;
}

btRigidBody* RigidBodyComponent::GetRigidBody()
{
	return mRigidBody;
}
	