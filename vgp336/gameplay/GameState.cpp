#include "GameState.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.AddService<TerrainService>();// "../../Assets/Settings/terrain.settings");
	mGameWorld.Initialize(1000);

	mGameWorld.CreateGameObject("../../Assets/Templates/follow_camera.json");
	//mGameWorld.CreateGameObject("../../Assets/Templates/fps_camera.json");
	//mGameObjectHandle = mGameWorld.CreateGameObject("../../Assets/Templates/test.json")->GetHandle();
	auto player = mGameWorld.CreateGameObject("../../Assets/Templates/player.json");
	player->SetName("Player");
	player->GetComponent<TransformComponent>()->position = { 100.0f,0.0f,100.0f };
	
	//auto gameObject = mGameWorld.CreateGameObject("../../Asserts/Templates/test.json");
	//mGameObjectHandle = gameObject->GetHandle();
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	auto gameObject = mGameWorld.GetGameObject(mGameObjectHandle);
	if (gameObject != nullptr)
		gameObject->DebugUI();

	mGameWorld.Render();

	auto& camera = mGameWorld.GetService<CameraService>()->GetCamera();
}

void GameState::DebugUI()
{
	mGameWorld.DebugUI();
}
