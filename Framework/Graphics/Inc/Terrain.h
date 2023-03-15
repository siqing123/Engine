#pragma once

//#include "Camera.h"
//#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "MeshTypes.h"
#include "Material.h"
//#include "PixelShader.h"
//#include "VertexShader.h"
//#include "Sampler.h"
#include "Texture.h"

namespace Storm::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(std::filesystem::path fileName, float heightScale);

		Mesh mesh;

		size_t mRows = 0;
		size_t mColumns = 0;
	};
}