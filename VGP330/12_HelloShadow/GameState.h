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

	NardaEngine::Graphics::RenderGroup mCharacter;
	NardaEngine::Graphics::RenderGroup mCharacter2;
	NardaEngine::Graphics::RenderGroup mCharacter3;
	NardaEngine::Graphics::RenderObject mSphere01;
	NardaEngine::Graphics::RenderObject mSphere02;
	NardaEngine::Graphics::RenderObject mGround;

	NardaEngine::Graphics::StandardEffect mStandardEffect;
	NardaEngine::Graphics::ShadowEffect mShadowEffect;

};