#include "Precompiled.h"
#include "TerrainService.h"

using namespace Storm;

void Storm::TerrainService::Initialize()
{
	mTerrain.Initialize("../../Assets/Heightmaps/HeightMap.raw", 25.0f);

	mTerrainRenderObject.diffuseMap.Initialize("../../Assets/Images/grass_2048.jpg");
	mTerrainRenderObject.specularMap.Initialize("../../Assets/Images/ground.jpg");
	mTerrainRenderObject.material.ambient = { 0.8f,0.8f,0.8f,1.0f };
	mTerrainRenderObject.material.diffuse = { 0.8f,0.8f,0.8f,1.0f };
	mTerrainRenderObject.material.specular= { 0.5f,0.5f,0.5f,1.0f };
	mTerrainRenderObject.material.power=10.0f;
	mTerrainRenderObject.meshBuffer.Initialize(mTerrain.mesh);
}

void Storm::TerrainService::Terminate()
{
	mTerrainRenderObject.Terminate();
}

void Storm::TerrainService::DebugUI()
{
	ImGui::Begin("Terrain Service", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("Material##Terrain", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Terrain", &mTerrainRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Terrain", &mTerrainRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Terrain", &mTerrainRenderObject.material.specular.r);
		ImGui::DragFloat("Power##Terrain", &mTerrainRenderObject.material.power,1.0f,1.0f,100.0f);
	}

	ImGui::End();
}

float TerrainService::GetHeight(const Math::Vector3& position) const
{
	const int x = static_cast<int>(position.x);
	const int z = static_cast<int>(position.z);

	if (x < 0 || z < 0 || x + 1 >= mTerrain.mColumns || z + 1 >= mTerrain.mRows)
		return 0.0f;

	const uint32_t bottomLeft = static_cast<uint32_t>((x + 0) + (z + 0) * mTerrain.mColumns);
	const uint32_t topLeft = static_cast<uint32_t>((x + 0) + (z + 1) * mTerrain.mColumns);
	const uint32_t bottomRight = static_cast<uint32_t>((x + 1) + (z + 0) * mTerrain.mColumns);
	const uint32_t topRight = static_cast<uint32_t>((x + 1) + (z + 1) * mTerrain.mColumns);

	const float u = position.x - x;
	const float v = position.z - z;

	float height = 0.0f;
	if (u > v) // bottom-right triangle
	{
		const auto& a = mTerrain.mesh.vertices[topLeft];
		const auto& b = mTerrain.mesh.vertices[topRight];
		const auto& c = mTerrain.mesh.vertices[bottomLeft];
		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * v) + (deltaAC * (1 - u));
	}
	else // top-left triangle
	{
		const auto& a = mTerrain.mesh.vertices[topLeft];
		const auto& b = mTerrain.mesh.vertices[topRight];
		const auto& c = mTerrain.mesh.vertices[bottomLeft];
		const auto deltaAB = b.position.y - a.position.y;
		const auto deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * u) + (deltaAC * (1 - v));
	}

	return height;
}


