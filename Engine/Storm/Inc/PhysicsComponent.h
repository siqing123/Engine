#pragma once
#include "Component.h"

namespace Storm
{
	class TransformComponent;
	class RigidBodyComponent;

	class PhysicsComponent final :public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Physics);
		

		void Initialize() override;
		void Terminate() override;
		void UpdatePhysics();
		void DebugUI() override;
	

	private:
		 TransformComponent* mTransformComponent = nullptr;
		 RigidBodyComponent* mRigidBodyComponent = nullptr;
	};
}