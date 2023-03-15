#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Transform.h"
#include "Model.h"

namespace Storm::Graphics
{
	struct IAnimator;
	class RenderObject final
	{
	public:
		void Terminate();

		Transform transform;
		Material material;

		Texture diffuseMap;
		Texture specularMap;
		Texture displacementMap;
		Texture normalMap;

		const Skeleton* skeleton = nullptr;
		const IAnimator* animator = nullptr;

		bool useAdditiveBlend = false;
		MeshBuffer meshBuffer;
	};

	using RenderGroup = std::vector<RenderObject>;
	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const IAnimator* animator = nullptr);

	void CleanUpRenderGroup(RenderGroup& renderGroup);

}
