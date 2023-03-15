#include "Precompiled.h"
#include "RenderObject.h"

using namespace Storm;
using namespace Storm::Graphics;

void RenderObject::Terminate()
{
	diffuseMap.Terminate();
	specularMap.Terminate();
	displacementMap.Terminate();
	normalMap.Terminate();

	meshBuffer.Terminate();
}

RenderGroup Storm::Graphics::CreateRenderGroup(const Model& model, const IAnimator* animator)
{
	RenderGroup renderGroup;
	renderGroup.reserve(model.meshData.size());

	auto TryLoadTexture = [](auto& texture, const auto& textureName)
	{
		if (!textureName.empty())
		{
			texture.Initialize(textureName);
		}
	};



	for (auto& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		auto& materialData = model.materialData[meshData.materialIndex];
		renderObject.material = materialData.material;
		TryLoadTexture(renderObject.diffuseMap, materialData.diffuseMapName);
		TryLoadTexture(renderObject.displacementMap, materialData.displacementMapName);
		TryLoadTexture(renderObject.normalMap, materialData.normalMapName);
		TryLoadTexture(renderObject.specularMap, materialData.specularMapName);

		renderObject.skeleton = model.skeleton.get();
		renderObject.animator = animator;
		//renderObject.animationclip = &model.animationSet[0];


		renderObject.meshBuffer.Initialize(meshData.mesh);
	}

	return renderGroup;

}

void Storm::Graphics::CleanUpRenderGroup(RenderGroup& renderGroup)
{
	for (auto& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}
