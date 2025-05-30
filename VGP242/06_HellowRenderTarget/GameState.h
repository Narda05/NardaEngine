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
	void UpdateCamera(float deltaTime);

	NardaEngine::Graphics::Camera mCamera;

	//Render GPU communication 
	NardaEngine::Graphics::ConstantBuffer mTransformBuffer; 
	NardaEngine::Graphics::VertexShader mVertexShader;
	NardaEngine::Graphics::PixelShader mPixelShader;
	NardaEngine::Graphics::Sampler mSampler;


	//oject
	NardaEngine::Math::Matrix4 mWorldMat = NardaEngine::Math::Matrix4::Identity;
	NardaEngine::Graphics::MeshBuffer mObject;
	NardaEngine::Graphics::TextureId mTextureId;

};