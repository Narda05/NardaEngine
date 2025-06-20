#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "MeshBuffer.h"
#include "TextureManager.h"


namespace NardaEngine::Graphics
{
	class Camera; 

	class SimpleTextureEffect
	{
	public:
		struct RenderData
		{
			MeshBuffer mesh; 
			Math::Matrix4 matWorld;
			TextureId textureId; 
		};

		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderData& renderData);

		void SetCamera(const Camera& camera);

	private:
		ConstantBuffer mTrandformBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		const Camera* mCamera = nullptr;
	};
}