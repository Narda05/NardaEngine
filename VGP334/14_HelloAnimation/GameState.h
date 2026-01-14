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
	NardaEngine::Graphics::DirectionalLight mDirectionalLight;

	//First Render Object
	NardaEngine::Graphics::RenderObject mRenderObject;

	NardaEngine::Graphics::StandardEffect mStandardEffect;

	NardaEngine::Graphics::Animation mAnimation;
	float mAnimationTime = 0.0f;

};