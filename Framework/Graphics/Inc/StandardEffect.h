#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderObject.h"
#include "DirectionalLight.h"

namespace NardaEngine::Graphics
{
	class Camera; 
	class RenderObject; 

	class StandardEffect final
	{
	public: 
		void Initialize(const std::filesystem::path& path);
		void Terminate(); 
		
		void Begin(); 
		void End(); 

		void Render(const RenderObject& renderObject); 

		void SetCamera(const Camera& camera); 
		void SetDirectionalLight(const DirectionalLight& directionalLight);

		void DebugUI(); 

	private: 
		struct TransformData 
		{
			Math::Matrix4 wvp; // world view projetion
			Math::Matrix4 world; // world matrix
			Math::Vector3 viewPosition; // position of the view item (camera)
			float padding = 0.0f; // padding to make the structure 16 byte aligned
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;


		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		VertexShader mVertexShader; 
		PixelShader mPixelShader; 

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
	};
}