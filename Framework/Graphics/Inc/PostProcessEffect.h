#pragma once
#include "Effect.h"
#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderTarget.h"
#include "Sampler.h"

namespace Storm::Graphics
{
	class PostProcessEffect final : public Effect
	{
	public:
		enum Mode
		{
			Blur,
			Emboss,
			Bloom
		};



		void Initialize() override;
		void Terminate() override;

		void Begin() override;
		void End() override;
		void Render(const RenderObject& renderObject) override;

		void SetTexture(const Texture* texture, uint32_t slot);
		void SetMode(Mode mode);
	
		const Texture& GetBaseTexture() const { return mBaseRenderTarget; }
		
		int& TextureDimensions() { return mTextureDimensions; }
		bool& UseBlurEffect() { return mUseBlurEffect; }
		bool& UseEmbossEffect() { return mUseEmbossEffect; }
		float& Embossamount() { return mEmbossamount; }


	private:
		struct PostProcessData
		{
			int mode = 0;
			int textureDimensions = 300;
			int useBlurEffect = 0;
			int useEmbossEffect = 0;
			float embossamount = 3;
			int padding[3];

		};

		using PostProcesssBuffer = Storm::Graphics::TypedConstantBuffer<PostProcessData>;

		PostProcesssBuffer mPostProcessBuffer;
		PostProcessData mPostProcessData;

		Storm::Graphics::RenderTarget mBaseRenderTarget;

		Storm::Graphics::VertexShader mPostProcessVertexShader;
		Storm::Graphics::PixelShader mPostProcessPixelShader;

		Sampler mSampler;
		std::array<const Texture*, 2> mTextures = {};


		int mTextureDimensions = 300;
		bool mUseBlurEffect = 0;
		bool mUseEmbossEffect = 0;
		float mEmbossamount = 3.0f;

	};



}
