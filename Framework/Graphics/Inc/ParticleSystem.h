#pragma once
#include "Common.h"
#include "Texture.h"
#include "Particle.h"
#include "Random.h"
#include "Camera.h"
#include "Colors.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MeshTypes.h"
#include "SimpleEffect.h"

struct ParticleProps
{

	Storm::Math::Vector3 position;
	Storm::Math::Vector3 velocity;
	Storm::Math::Vector3 velocityvariation{ 1.0f,1.0f,1.0f };
	float lifetime = 1.0f;
	float size = 1.0f;
	Storm::Graphics::Color color = Storm::Graphics::Colors::White;
	Storm::Graphics::Color startColor = Storm::Graphics::Colors::White;
	Storm::Graphics::Color endColor = Storm::Graphics::Colors::White;


};


namespace Storm::Graphics
{
	class ParticleSystem
	{
	public:
		void Initialize();
		void Render();
		void Terminate();
		void Emit(const ParticleProps& particlepros);
		void SetCamera(const Camera& camera);
		void UpdateParticles(float deltaTime);


	private:
		std::vector<Particle> particlepool;
		Math::Vector3 origin;
		uint32_t poolIndex = 999;

		const Camera* mCamera = nullptr;
		SimpleEffect  mSimpleEffect;

		Particle particle;
	};
}
