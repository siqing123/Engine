#include "Precompiled.h"
#include "ParticleSystem.h"
#include "VertexTypes.h"
#include "MeshBuilder.h"

using namespace Storm;
using namespace Storm::Graphics;

void Storm::Graphics::ParticleSystem::Initialize()
{
	const size_t poolSize = 5000;
	poolIndex = 0;

	particlepool.resize(poolSize, particle);
	mSimpleEffect.Initialize();
}

void Storm::Graphics::ParticleSystem::Render()
{

	mSimpleEffect.SetCamera(*mCamera);
	for (auto& particle : particlepool)
	{
		if (particle.IsActive)
		{

			mSimpleEffect.Begin();
			mSimpleEffect.Render(particle);
			mSimpleEffect.End();

		}

	}
}

void Storm::Graphics::ParticleSystem::Terminate()
{
	mSimpleEffect.Terminate();
}


void Storm::Graphics::ParticleSystem::Emit(const ParticleProps& particlepros)
{
	std::mt19937 generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
	Particle& particle = particlepool[poolIndex];
	particle.IsActive = true;
	particle.transform.position = particlepros.position;
	particle.transform.rotation = Math::Quaternion::RotationEuler(Math::Vector3{ (generator() % 6) * 2.0f * Math::Constants::Pi });

	particle.velocity = particlepros.velocity;
	particle.velocity.x += particlepros.velocityvariation.x * (generator() % 6 - 1.0f);
	particle.velocity.y += particlepros.velocityvariation.y * (generator() % 6 - 1.0f);
	particle.velocity.z += particlepros.velocityvariation.z * (generator() % 6 - 1.0f);

	particle.lifeTime = particlepros.lifetime;
	particle.lifeRemain = particlepros.lifetime;
	particle.color = particlepros.color;
	particle.startColor = particlepros.startColor;
	particle.endColor = particlepros.endColor;
	//particle.color.a = particle.color.a * life;

	particle.size = particlepros.size;

	poolIndex = (++poolIndex) % particlepool.size();
}

void Storm::Graphics::ParticleSystem::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void Storm::Graphics::ParticleSystem::UpdateParticles(float deltaTime)
{
	for (auto& particle : particlepool)
	{
		if (!particle.IsActive)
		{
			continue;
		}
		if (particle.lifeRemain <= 0.0f)
		{
			particle.IsActive = false;
			continue;
		}

		particle.lifeRemain -= deltaTime;
		particle.transform.position += particle.velocity * deltaTime;
		particle.transform.rotation += Math::Quaternion::RotationEuler(Math::Vector3{ 0.01f * deltaTime });
		//particle.meshBuffer.Update();

	}

}

