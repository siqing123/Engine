#include "Precompiled.h"
#include "RenderService.h"

#include "AnimatorComponent.h"
#include "CameraService.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "ModelComponent.h"
#include "TerrainService.h"
#include "TransformComponent.h"

using namespace Storm;
using namespace Storm::Input;
using namespace Storm::Graphics;


void RenderService::Initialize()
{
	Graphics::GraphicsSystem::Get()->SetClearColor(Colors::White);

	mCameraService = GetWorld().GetService<CameraService>();
	mTerrainService = GetWorld().GetService<TerrainService>();

	//mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
	//mCamera.SetLookAt({ 0.0f,1.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionalLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionalLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mStandardEffect.Initialize();
	//mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mStandardEffect.UseSpecularFlag() = true;
	mStandardEffect.UseDisplacementFlag() = true;
	mStandardEffect.UseNorFlag() = true;
	mStandardEffect.UseShadowFlag() = true;
	mStandardEffect.DisplacementWeight() = 0.3f;
	mStandardEffect.DepthBias() = 0.000222f;
	
	mTerrainEffect.Initialize();
	mTerrainEffect.SetLightCamera(&mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
	mTerrainEffect.UseShadowFlag() = true;
	mTerrainEffect.DepthBias() = 0.000222f;

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void RenderService::Terminate()
{
	mShadowEffect.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
	//auto inputSystem = InputSystem::Get();
	////Camera control---------------------------------
	//const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	//const float turnspeed = 0.3f;
	//if (inputSystem->IsKeyDown(KeyCode::W))
	//	mCamera.Walk(movespeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::A))
	//	mCamera.Strafe(-movespeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::S))
	//	mCamera.Walk(-movespeed * deltaTime);
	//if (inputSystem->IsKeyDown(KeyCode::D))
	//	mCamera.Strafe(movespeed * deltaTime);

	//if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	//{
	//	mCamera.Yaw(inputSystem->GetMouseMoveX() * turnspeed * deltaTime);
	//	mCamera.Pitch(inputSystem->GetMouseMoveY() * turnspeed * deltaTime);
	//}

	mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
	auto& camera = mCameraService->GetCamera();
	mStandardEffect.SetCamera(camera);
	mTerrainEffect.SetCamera(camera);
	
	for (auto& entry : mRenderEntries)
	{
		for (auto& renderObject : entry.renderGroup)
		{
			auto angles = entry.modelComponent->GetRotation();
			auto rotation = Math::Quaternion::RotationEuler(angles);
			auto transform=*(static_cast<const Graphics::Transform*>(entry.transformComponent));
			transform.rotation = rotation * transform.rotation;
			renderObject.transform = transform;
		}
	}

	mShadowEffect.Begin();
	for (auto& entry : mRenderEntries)
		mShadowEffect.RenderGroup(entry.renderGroup);
	if (mTerrainService)
		mShadowEffect.Render(mTerrainService->GetTerrainRenderObject());
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (auto& entry : mRenderEntries)
		mStandardEffect.RenderGroup(entry.renderGroup);
	mStandardEffect.End();

	if (mTerrainService)
	{
		mTerrainEffect.Begin();
		mTerrainEffect.Render(mTerrainService->GetTerrainRenderObject());
		mTerrainEffect.End();
	}

	SimpleDraw::AddGroundPlane(10.0f, Colors::LightGray);
	SimpleDraw::AddTransform(Math::Matrix4::Translation({ 0.0f,0.01f,0.0f }));
	SimpleDraw::Render(camera);
}

void RenderService::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Settings"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Use Specular Map", &mStandardEffect.UseSpecularFlag());
		ImGui::Checkbox("Use Displacement Map", &mStandardEffect.UseDisplacementFlag());
		ImGui::Checkbox("Use Normal Map", &mStandardEffect.UseNorFlag());
		ImGui::Checkbox("Use shadow Map", &mStandardEffect.UseShadowFlag());

		
		ImGui::DragFloat("Displacment Weight", &mStandardEffect.DisplacementWeight(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Depth Bias", &mStandardEffect.DepthBias(), 0.001f, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Camera"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Image(mShadowEffect.GetDepthMap().GetRawData(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f,1.0f });
	}

	ImGui::End();
}

void RenderService::Register(const ModelComponent* modelComponent)
{
	Entry& entry = mRenderEntries.emplace_back();
	auto& gameObject = modelComponent->GetOwner();

	entry.animatorComponent = gameObject.GetComponent<AnimatorComponent>();
	entry.modelComponent = modelComponent;
	entry.transformComponent = gameObject.GetComponent<TransformComponent>();

	const IAnimator* animator = nullptr;
	if (entry.animatorComponent)
		animator = &entry.animatorComponent->GetAnimator();

	entry.renderGroup = CreateRenderGroup(modelComponent->GetModel(), animator);
	
}

void RenderService::Unregister(const ModelComponent* modelComponent)
{
	auto match = [&](const auto& entry) {return entry.modelComponent == modelComponent; };
	auto iter = std::find_if(mRenderEntries.begin(), mRenderEntries.end(), match);
	if (iter != mRenderEntries.end())
	{
		Entry& entry = *iter;
		CleanUpRenderGroup(entry.renderGroup);
		mRenderEntries.erase(iter);
	}
}
