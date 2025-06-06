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
	NardaEngine::Graphics::Camera mRenderTargetCamera;

	//GPU communication 
	NardaEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	//render object
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mObject0;
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mObject1;

	// render target
	NardaEngine::Graphics::RenderTarget mRenderTarget;

};