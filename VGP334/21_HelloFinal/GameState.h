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
	
	//Update helpers
	void UpdateCamera(float deltaTime);
	
	//Scene logic 
	void UpdateScene(float deltaTime);
	void UpdateRunningSection(float deltaTime);
	void UpdateTalkingSection(float deltaTime);
	void UpdateFallSection(float deltaTime);
	void UpdateTogetherSection(float deltaTime);
	void UpdateFadeSection(float deltaTime);

private:

	//Scine timer in seconds 
	float mSceneTime = 0.0f;
	float mSceneDuration = 47.0f;
	bool mIsPlaying = true;

	// Key sections
	bool mIsRunningStarted = false;
	bool mTalkingStarted = false;
	bool mFallStarted = false;
	bool mTogetherStarted = false;
	bool mFadeStarted = false;


	//Cameras
	NardaEngine::Graphics::Camera mDebugCamera;
	NardaEngine::Graphics::Camera mCameraRunLegs;
	
	const NardaEngine::Graphics::Camera* mMainCamera = nullptr;

	bool mUseDebugCamera = false;

	// Camera animations
	NardaEngine::Graphics::Animation mCameraRunAnimation;
	float mCameraRunAnimationTime = 0.0f;

	//Light
	NardaEngine::Graphics::DirectionalLight mDirectionalLight;
	NardaEngine::Graphics::StandardEffect mStandardEffect;

	//Characters
	NardaEngine::Graphics::RenderGroup mCharacter04;
	NardaEngine::Graphics::Animator mAnimator04;

	NardaEngine::Graphics::RenderGroup mCharacter05;
	NardaEngine::Graphics::Animator mAnimator05;



	int mRunClip04 = 0;
	int mRunClip05 = 0;

	float mAnimationSpeed04 = 0.65f;
	float mAnimationSpeed05 = 0.65f;

	//Animation clips 
	NardaEngine::Math::Vector3 mRunStartPos04 = { -8.0f, 0.0f, -0.4f };
	NardaEngine::Math::Vector3 mRunEndPos04 = { 2.0f, 0.0f, -0.4f };

	NardaEngine::Math::Vector3 mRunStartPos05 = { -8.0f, 0.0f,  0.4f };
	NardaEngine::Math::Vector3 mRunEndPos05 = { 2.0f, 0.0f,  0.4f };

	float mRunSectionStart = 0.0f;
	float mRunSectionEnd = 4.82f;


	// Debug 
	bool mDrawSkeleton = false;
	float mRunSpeed = 2.2f;

};