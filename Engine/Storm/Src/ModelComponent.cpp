#include"Precompiled.h"
#include"ModelComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace Storm;
using namespace Storm::Graphics;

MEMORY_POOL_DEFINE(ModelComponent, 1000);


void ModelComponent::Initialize()
{
	const char* modelFileName = mFileName.c_str();
	ModelIO::LoadModel(modelFileName, mModel);
	ModelIO::LoadMaterial(modelFileName, mModel);
	ModelIO::LoadSkeleton(modelFileName, mModel);
	ModelIO::LoadAnimationSet(modelFileName, mModel);

	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Register(this);
	
}

void ModelComponent::Terminate()
{
	auto renderService = GetOwner().GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

