#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace Storm;
using namespace Storm::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sInstance;
}


void ModelManager::StaticInitializa()
{
	ASSERT(sInstance == nullptr, "ModelManager --Manager already initialized.");
	sInstance = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
	sInstance.reset();
}

ModelManager* ModelManager::Get()
{
	ASSERT(sInstance != nullptr, "ModelManager ==Manager not initialized");
	return sInstance.get();
}

//Storm::Graphics::ModelManager::~ModelManager()
//{
//	for (auto& [id, Model] : mInventory)
//		Model->Terminate();
//	mInventory.clear();
//}

ModelID ModelManager::LoadModel(std::filesystem::path fileName)
{
	const auto modelID = std::filesystem::hash_value(fileName);
	auto [iter, success] = mInventory.insert({ modelID,nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(fileName, *modelPtr);
		ModelIO::LoadMaterial(fileName, *modelPtr);
		ModelIO::LoadSkeleton(fileName, *modelPtr);
		ModelIO::LoadAnimations(fileName, *modelPtr);
	}
	return modelID;
}

const Model* ModelManager::GetModel(ModelID id) const
{
	const auto iter = mInventory.find(id);
	if (iter == mInventory.end())
		return nullptr;
	return iter->second.get();
}

