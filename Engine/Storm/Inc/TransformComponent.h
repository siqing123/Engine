#pragma once

#include "Component.h"

namespace Storm
{
	class TransformComponent final
		: public Component
		, public Graphics::Transform
	{
	public:
		SET_TYPE_ID(ComponentId::Transform);
			MEMORY_POOL_DECLARE

			void DebugUI() override;


			//	Math::Vector3& Position() { return mTransform.position; }
			//	Math::Quaternion& Rotation() { return mTransform.rotation; }
			//	Math::Vector3& Scale() { return mTransform.scale; }

			//	const Math::Matrix4& GetMatrix4() const { mTransform.GetMatrix4(); }

			//private:
			//	Graphics::Transform mTransform;
	};
}