#pragma once
#include "Service.h"

namespace Storm
{
	class PhysicsComponent;
	class TransformComponent;
	class RigidBodyComponent;
	class ColliderComponent;
	
	class PhysicsService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void DebugUI() override;
		void SetGravity(float gravity) ;

	private:
		friend class PhysicsComponent;

		void Register(PhysicsComponent* physicsComponent);
		void Unregister(PhysicsComponent* physicsComponent);

		struct Entry
		{
			PhysicsComponent* physicsComponent = nullptr;
			TransformComponent* transformComponent = nullptr;
		};

		using PhysicsEntities = std::vector<Entry>;
		PhysicsEntities mPhysicsEntries;
		
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration * mCollisionConfiguration = nullptr;
		btDiscreteDynamicsWorld * mDynamicWorld = nullptr;
		btSequentialImpulseConstraintSolver	* mSolver = nullptr;

		bool mRenderDebugUI = false;
		float mGravity = -9.81f;
	};
}