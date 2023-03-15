#pragma once
#include <Storm/Inc/Storm.h>

class GameState :public Storm::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	Storm::GameWorld mGameWorld;
	Storm::GameObjectHandle mGameObjectHandle;

	//Storm::Graphics::Terrain mTerrain;
	//Storm::Graphics::TerrainEffect mTerrainEffect;
};