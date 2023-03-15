#pragma once

#include "Service.h"

namespace Storm
{
	class AnimatorComponent;
	class ModelComponent;
	class TransformComponent;

	class CameraService;
	class TerrainService;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;

	private:
		friend class ModelComponent;

		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		struct Entry
		{
			const AnimatorComponent* animatorComponent = nullptr;
			const ModelComponent* modelComponent=nullptr;
			const TransformComponent* transformComponent = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		std::vector<Entry> mRenderEntries;

		const CameraService* mCameraService = nullptr;
		const TerrainService* mTerrainService = nullptr;

		//Storm::Graphics::Camera mCamera;
		Storm::Graphics::DirectionalLight mDirectionalLight;

		Storm::Graphics::StandardEffect mStandardEffect;
		Storm::Graphics::TerrainEffect mTerrainEffect;
		Storm::Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;
	};
}