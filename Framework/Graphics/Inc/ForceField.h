#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderObject.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Sampler.h"
#include "BlendState.h"

namespace NardaEngine::Graphics
{
	class Camera;
	class RenderObject;
	class RenderGroup;
	class Texture;
	class ForceField final
	{
	public:
		void Initialize(const std::filesystem::path& path);
		void Terminate();

		void Begin(float time);
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderGroup& renderGroup);

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetLightCamera(const Camera& camera);

		void DebugUI();
	private:
		struct ForceFieldData
		{
			// Keep data laid out in 16-byte rows to match HLSL cbuffer packing.
			int   mode = 0;       // 4 bytes
			float param0 = 0.0f;  // 4 bytes
			float param1 = 0.0f;  // 4 bytes
			float param2 = 0.0f;  // 4 bytes  -> first 16 bytes

			float param3 = -1.0f; // 4 bytes
			float pad0 = 0.0f;    // 4 bytes
			float pad1 = 0.0f;    // 4 bytes
			float pad2 = 0.0f;
		};

		struct TransformData
		{
			Math::Matrix4 wvp; // world view projetion
			Math::Matrix4 world; // world matrix
			Math::Matrix4 lwvp; // world view projection OF THE LIGHT OBJECT FOR SHADOWS
			Math::Vector3 viewPosition; // position of the view item (camera)
			float padding = 0.0f; // padding to make the structure 16 byte aligned
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useSpecMap = 1;
			int useNormalMap = 1;
			int useBumpMap = 1;
			int useShadowMap = 1;
			float bumpWeight = 0.1f;
			float depthBias = 0.000003f;
			float padding = 0.0f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;


		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		using MaterialBuffer = TypedConstantBuffer<Material>;
		MaterialBuffer mMaterialBuffer;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		using ForceFieldBuffer = TypedConstantBuffer<ForceFieldData>;
		ForceFieldBuffer mForceFieldBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		BlendState mBlendState;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Camera* mLightCamera = nullptr;
		const Texture* mShadowMap = nullptr;

	};

}