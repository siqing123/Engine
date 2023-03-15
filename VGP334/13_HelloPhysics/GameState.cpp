#include "GameState.h"

using namespace Storm;
using namespace Storm::Graphics;
using namespace Storm::Input;
using namespace Storm::Math;
using namespace Storm::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition(Vector3{ 0.0f,1.0f,-3.0f });
	mCamera.SetLookAt({ 0.0f,1.0f,0.0f });
	mCamera.SetMode(Camera::ProjectionMode::Perspective);
	mCamera.SetSize(40.0f, 20.0f);

	PhysicsWorld::Settings settings
	{
		{ 0.0f,-9.8f,0.0f },
		1.0f / 60.0f,
		0.1f,
		10
	};


	mPhysicsWorld.Initialize(settings);
	//mPhysicsWorld.AddPlane({ Vector3::YAxis,0.0f });
	mPhysicsWorld.AddOBB({
		{0.0f,2.0f,0.0f},
		{3.0f,0.2f,2.0f},
		Quaternion::RotationEuler({0.0f,0.0f,-5.0f})}
	);

	const char* modelFileName = "../../Assets/Models/Duck/duck.model";
	ModelIO::LoadModel(modelFileName, model);
	ModelIO::LoadMaterial(modelFileName, model);
	//ModelIO::LoadSkeleton(modelFileName, model);

	mHuman = CreateRenderGroup(model);
	mHuman[0].transform.scale = Vector3{ 0.1f };
	mStandardEffect.Initialize();
	mShadowEffect.Initialize();
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionLight);
	mShadowEffect.SetDirectionalLight(mDirectionLight);

	mDirectionLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
	mDirectionLight.ambient = { 0.1f,0.1f,0.1f,1.0f };
	mDirectionLight.diffuse = { 0.75f,0.75f,0.75f,1.0f };
	mDirectionLight.specular = { 0.3f,0.3f,0.3f,1.0f };

	mCullNone.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mWireframe.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Wireframe);

}

void GameState::Terminate()
{
	mWireframe.Terminate();
	mCullNone.Terminate(); 

	CleanUpRenderGroup(mHuman);
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	//Camera control---------------------------------
	const float movespeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnspeed = 0.3f;
	if (inputSystem->IsKeyDown(KeyCode::W))
	{

		mCamera.Walk(movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-movespeed * deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(movespeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{

		mCamera.Yaw(inputSystem->GetMouseMoveX() * turnspeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * turnspeed * deltaTime);

	}
	mFPS = 1.0f / deltaTime;
	mPhysicsWorld.Update(deltaTime * speed);

}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();
	SimpleDraw::AddGroundPlane(10.0f, Colors::LightGray);
	SimpleDraw::AddTransform(Matrix4::Translation({ 0.0f,0.01f,0.0f }));
	SimpleDraw::Render(mCamera);
	if (mDrawModel)
	{
		mShadowEffect.Begin();
		for (auto& t : mTetrahedrons)
		{
			auto worldMatrix = t.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mHuman)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}
			mShadowEffect.RenderGroup(mHuman);
		}

		mShadowEffect.End();
		mStandardEffect.Begin();
		for (auto& t : mTetrahedrons)
		{
			auto worldMatrix = t.GetWorldMatrix();
			auto translation = GetTranslation(worldMatrix);
			auto rotation = Quaternion::RotationMatrix(worldMatrix);
			for (auto& robj : mHuman)
			{
				robj.transform.position = translation;
				robj.transform.rotation = rotation;
			}
			mStandardEffect.RenderGroup(mHuman);
		}

		mStandardEffect.End();
	}
}
//namespace
//{
//	float RandomFloat(float min, float max)
//	{
//		return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
//	}
//}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", mFPS);
	if (ImGui::Button("Boom!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; ++i)
		{
			Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.1f;
			vel.y = Abs(vel.y);

			auto particle = mPhysicsWorld.AddParticle();
			particle->SetPosition({ 0.0f,1.0f,0.0f });
			particle->SetVelocity(vel);
			particle->radius = Math::Random::UniformFloat(1.02f, 1.1f);
			particle->invMass = 1.0f;
			particle->bounce = Math::Random::UniformFloat(0.5f, 0.95f);

		}

	}
	if (ImGui::Button("Sticks!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; ++i)
		{
			Storm::Math::Vector3 vel1 = Math::Random::OnUnitSphere() * 0.1f;
			vel1.y = Abs(vel1.y);
			Storm::Math::Vector3 vel2 = Math::Random::OnUnitSphere() * 0.1f;
			vel2.y = Abs(vel2.y);

			auto particle1 = mPhysicsWorld.AddParticle();
			particle1->SetPosition({ -0.1f,1.0f,0.0f });
			particle1->SetVelocity(vel1);
			particle1->radius = 0.02f;
			particle1->invMass = 1.0f;

			auto particle2 = mPhysicsWorld.AddParticle();
			particle2->SetPosition({ 0.1f,1.0f,0.0f });
			particle2->SetVelocity(vel2);
			particle2->radius = 0.02f;
			particle2->invMass = 1.0f;

			mPhysicsWorld.AddConstraint<Physics::Spring>(particle1, particle2);

		}

	}
	if (ImGui::Button("Ball and Chain!"))
	{
		mPhysicsWorld.Clear(true);
		Math::Vector3 position{ 0.0f,3.0f,0.0f };
		Physics::Particle* last = nullptr;
		for (int i = 0; i < 10; ++i)
		{
			auto p = mPhysicsWorld.AddParticle();
			p->SetPosition(position);
			p->SetVelocity(Math::Vector3::Zero);
			p->radius = (i == 9) ? 0.5f : 0.02f;
			p->invMass = (i == 9) ? 0.1f : 1.0f;
			if (i == 0)
			{
				mPhysicsWorld.AddConstraint<Physics::Fixed>(p);
			}
			else
			{
				mPhysicsWorld.AddConstraint<Physics::Spring>(p, last);
			}

			position.x += 0.5f;
			last = p;

		}

	}
	if (ImGui::Button("Tetrahedron!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; ++i)
		{
			auto p1 = mPhysicsWorld.AddParticle();
			p1->SetPosition({ 0.0f,1.0f,0.0f });
			p1->radius = 0.02f;
			p1->invMass = 1.0f;

			auto p2 = mPhysicsWorld.AddParticle();
			p2->SetPosition({ 0.0f,0.8f,0.1f });
			p2->radius = 0.02f;
			p2->invMass = 1.0f;

			auto p3 = mPhysicsWorld.AddParticle();
			p3->SetPosition({ 0.1f,0.8f,-0.1f });
			p3->radius = 0.02f;
			p3->invMass = 1.0f;

			auto p4 = mPhysicsWorld.AddParticle();
			p4->SetPosition({ -0.1f,0.8f,-0.1f });
			p4->radius = 0.02f;
			p4->invMass = 1.0f;

			Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.25f;
			vel.y = Abs(vel.y);
			p1->SetVelocity(vel);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p3, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p4, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p3, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p4, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p2, 0.2f);

		}

	}

	if (ImGui::Button("Cube!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; ++i)
		{
			auto p1 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p2 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p3 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p4 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p5 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p6 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p7 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			auto p8 = mPhysicsWorld.AddParticle(0.02f, 1.0f);
			p1->SetPosition({ 0.0f,0.0f,0.0f });
			p2->SetPosition({ 1.0f,0.0f,0.0f });
			p3->SetPosition({ 0.0f,1.0f,0.0f });
			p4->SetPosition({ 1.0f,1.0f,0.0f });
			p5->SetPosition({ 0.0f,0.0f,1.0f });
			p6->SetPosition({ 1.0f,0.0f,1.0f });
			p7->SetPosition({ 0.0f,1.0f,1.0f });
			p8->SetPosition({ 1.0f,1.0f,1.0f });


			Storm::Math::Vector3 vel = Math::Random::OnUnitSphere() * 0.5f;
			vel.y = Abs(vel.y);
			p1->SetVelocity(vel);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p2, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p4, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p3, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p1, 0.2f);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p5, p6, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p6, p8, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p8, p7, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p7, p5, 0.2f);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p5, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p6, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p7, 0.2f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p8, 0.2f);

			mPhysicsWorld.AddConstraint<Physics::Spring>(p1, p8, 0.35f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p3, p6, 0.35f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p4, p5, 0.35f);
			mPhysicsWorld.AddConstraint<Physics::Spring>(p2, p7, 0.35f);
		}

	}
	if (ImGui::Button("Cloth!"))
	{
		mPhysicsWorld.Clear(true);
		const size_t rows = 30;
		const size_t columns = 30;
		std::vector<Physics::Particle*> particles;
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; j++)
			{
				auto p1 = mPhysicsWorld.AddParticle();
				p1->SetPosition({ 0.0f + 0.2f * j,0.0f - 0.2f * i,0.0f });
				p1->radius = 0.02f;
				p1->invMass = 1.0f;

				particles.push_back(p1);
			}
		}
		mPhysicsWorld.AddConstraint <Physics::Fixed>(particles[0]);
		mPhysicsWorld.AddConstraint <Physics::Fixed>(particles[columns - 1]);

		for (uint32_t r = 0; r < rows; ++r)
		{

			for (uint32_t c = 0; c < columns - 1; ++c)
			{
				mPhysicsWorld.AddConstraint<Physics::Spring>(particles[r * columns + c], particles[r * columns + c + 1], 0.2f);
				if (r < rows - 1)
				{
					mPhysicsWorld.AddConstraint<Physics::Spring>(particles[r * columns + c], particles[(r + 1) * columns + c + 1], 0.283f);
				}
			}
		}
		for (int i = 0; i < particles.size() - columns; ++i)
		{
			mPhysicsWorld.AddConstraint<Physics::Spring>(particles[i], particles[i + columns], 0.2f);

		}

	}

	if (ImGui::Button("Draw Model Using Tetrahedron!!"))
	{
		mPhysicsWorld.Clear(true);
		mTetrahedrons.clear();
		for (int i = 0; i < 10; ++i)
		{
			auto& t = mTetrahedrons.emplace_back(mPhysicsWorld.AddTetrahedron());
			t.SetPosition({ 0.0f,1.0f,0.0f });
			Vector3 vel = Math::Random::OnUnitSphere() * 0.25f;
			vel.y = Abs(vel.y);
			t.SetVelocity(vel);

			Vector3 vel2 = Math::Random::OnUnitSphere() * 0.25f;
			t.a->SetVelocity(vel2);

		}
		mDrawModel = true;
	}

	ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 1.0f);


	if (ImGui::Checkbox("CullNone", &CullNone)|| ImGui::Checkbox("Wireframe", &Wireframe))
	{
		if (CullNone)
		{
			mCullNone.Set();
		}
		else if (Wireframe)
		{
			mWireframe.Set();
		}
		else
		{
			RasterizerState::Clear();
		}

	}



	//;


	ImGui::End();
	mPhysicsWorld.DebugUI();
}



