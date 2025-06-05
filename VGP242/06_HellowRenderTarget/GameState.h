#pragma once

#include <NardaEngine/Inc/SpEngine.h>

class GameState : public NardaEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct Object
	{
		NardaEngine::Math::Matrix4 matWorld = NardaEngine::Math::Matrix4::Identity;
		NardaEngine::Graphics::MeshBuffer meshBuffer;
		NardaEngine::Graphics::TextureId textureId = 0;

	};
	void UpdateCamera(float deltaTime);
	void RenderObject(const Object& object ,const NardaEngine::Graphics::Camera& camera);
	
	NardaEngine::Graphics::Camera mCamera;
	NardaEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication 
	NardaEngine::Graphics::ConstantBuffer mTransformBuffer; 
	NardaEngine::Graphics::VertexShader mVertexShader;
	NardaEngine::Graphics::PixelShader mPixelShader;
	NardaEngine::Graphics::Sampler mSampler;

	//render object
	Object mObject0;
	Object mObject1;
	

	// render target
	NardaEngine::Graphics::RenderTarget mRenderTarget;

};