#pragma once

#include "Component.h"

namespace Storm
{
	class RenderService;

	class ModelComponent final :public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);
		MEMORY_POOL_DECLARE

		void Initialize() override;
		void Terminate() override;

		Graphics::Model& GetModel() { return mModel; }
		const Graphics::Model& GetModel() const { return mModel; }

		const Math::Vector3& GetRotation() const { return mRotation; }

		void SetFileName(const char* fileName) { mFileName = fileName; }
		void SetRotation(const Math::Vector3& rotation) { mRotation = rotation; }

	private:
		//RenderService* mRenderService = nullptr;
		std::string mFileName;
		Graphics::Model mModel;
		Math::Vector3 mRotation = Math::Vector3::Zero;
	};
}