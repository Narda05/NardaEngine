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
	float mTime = 0.0f;

	void UpdateCamera(float deltaTime);
	
	NardaEngine::Graphics::Camera mCamera;
	NardaEngine::Graphics::DirectionalLight mDirectionalLight;
	NardaEngine::Graphics::BlendState mBlendState;

	NardaEngine::Graphics::RenderObject mSphere01;
	NardaEngine::Graphics::RenderObject mGround;

	NardaEngine::Graphics::ForceField mForceField;
	NardaEngine::Graphics::StandardEffect mStandardEffect;

	//defaults to control startup appearance
	float mNoiseScale = 2.8f;
	float mNoiseAmp = 0.40f;
	float mRimWidth = 0.80f;

	// Colors editable from UI (R,G,B,A)
	float mRimColor[4] = { 0.9568627f, 0.3254902f, 0.9764706f, 1.0f }; // inner rim (emissive)
	float mCenterColor[4] = { 0.6705882f, 0.2980392f, 1.0f,       1.0f };// center
	float mOuterRimColor[4] = { 0.6f,        1.0f,        0.2f,       1.0f };// outer rim (specular) default orange
};