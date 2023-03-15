#pragma once
#include "Component.h"

namespace Storm
{
	class TransformComponent;
	class ColliderComponent;

	class RigidBodyComponent final :public Component
	{
	public:
		SET_TYPE_ID(ComponentId::RigidBody);
		MEMORY_POOL_DECLARE

		void Initialize() override;
		void Terminate() override;

		void SetMass(float mass);

		bool IsDynamic() const;
		btRigidBody* GetRigidBody();
	private:
		const TransformComponent* mTransformComponent = nullptr;
		const ColliderComponent* mColliderComponent = nullptr;
		btRigidBody * mRigidBody = nullptr;
		float mMass = 0.0f;
		bool mIsDynamic = false;
	};
}