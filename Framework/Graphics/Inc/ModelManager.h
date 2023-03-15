#pragma once

#include "Model.h"

namespace Storm::Graphics
{
	using ModelID = std::size_t;

	class ModelManager final
	{
	public:
		static void StaticInitializa();
		static void StaticTerminate();
		static ModelManager* Get();

	public:

		ModelID LoadModel(std::filesystem::path fileName);

		const Model* GetModel(ModelID id) const;

	private:
		std::unordered_map<ModelID, std::unique_ptr<Model>> mInventory;
	};
}
