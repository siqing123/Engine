#pragma once

#include "Service.h"

namespace Storm
{

	class TerrainService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Terrain);

		//TerrainService(std::filesystem::path terrainSetting);

		void Initialize() override;
		void Terminate() override;

		void DebugUI() override;

		float GetHeight(const Math::Vector3& position) const;

		const Graphics::RenderObject& GetTerrainRenderObject() const { return mTerrainRenderObject; }

	private:
		Graphics::Terrain mTerrain;
		Graphics::RenderObject mTerrainRenderObject;
	};
}