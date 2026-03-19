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
	void UpdateRunningSection(float deltaTime); // 0.00 - 4.82 Characters runing
	void UpdateTalkingSection(float deltaTime); // 4.82 - 6.50 RunningToTurn transition
	void UpdateDiscussionSection(float deltaTime); // Discution scene
	void UpdateFallSection(float deltaTime);	// 17.00 - 22.00 Character falling
	void UpdateTogetherSection(float deltaTime); // Future: interaction scene
	void UpdateFadeSection(float deltaTime);	// End fade 

private:

	//Scine timer in seconds 
	float mSceneTime = 0.0f;
	float mSceneDuration = 47.0f;
	bool mIsPlaying = true;


	//Timing (per section)
	float mRunSectionStart = 0.0f;
	float mRunSectionEnd = 4.82f;

	float mTurnSectionStart = 4.82f;
	float mTurnSectionEnd = 6.50f;

	float mDiscussionSectionStart = 6.50f;
	float mDiscussionSectionEnd = 17.0f;

	float mFallSectionStart = 17.0f;
	float mFallSectionEnd = 22.0f;

	float mTogetherSectionStart = 22.0f;
	float mTogetherSectionEnd = 46.0f;

	float mFadeSectionStart = 46.0f;
	float mFadeSectionEnd = 47.0f;

	// Key sections (triggers)
	bool mIsRunningStarted = false;
	bool mTalkingStarted = false;
	bool mDiscussionStarted = false;
	bool mFallStarted = false;
	bool mTogetherStarted = false;
	bool mFadeStarted = false;

	bool mRunToTurnStarted = false;
	bool mPushStarted = false;
	bool mFallTogetherStarted = false;
	bool mFallingStarted = false;
	bool mFloatingStarted = false;
	bool mTalkCharactersPlaced = false;
	bool mFloatingFinalCameraStarted = false;

	//Audio
	NardaEngine::Audio::SoundId mIntroMusicId = 0;
	NardaEngine::Audio::SoundId mSecondPartMusicId = 0;
	NardaEngine::Audio::SoundId mBeastSfxId = 0;
	NardaEngine::Audio::SoundId mThunderSfxId = 0;
	NardaEngine::Audio::SoundId mDiscussionSfxId = 0;

	bool mIntroMusicStarted = false;
	bool mSecondPartMusicStarted = false;
	bool mBeastSfxStarted = false;
	bool mDiscussionSfxStarted = false;


	//Cameras (Cinematic)
	NardaEngine::Graphics::Camera mDebugCamera; // Free camera for debugging and testing
	NardaEngine::Graphics::Camera mCameraRunLegs;
	NardaEngine::Graphics::Camera mCameraFrontTalk;

	NardaEngine::Graphics::Camera mCameraArgue05;
	NardaEngine::Graphics::Camera mCameraArgue04;
	NardaEngine::Graphics::Camera mCameraAgueWide;
	NardaEngine::Graphics::Camera mCameraArgueOrbit;
	
	NardaEngine::Graphics::Camera mCameraPushWide;
	NardaEngine::Graphics::Camera mCameraFallSide;
	NardaEngine::Graphics::Camera mCameraFallTop;

	NardaEngine::Graphics::Camera mCameraFloatingFinal;
	
	const NardaEngine::Graphics::Camera* mMainCamera = nullptr;
	bool mUseDebugCamera = false;

	// Camera animations
	NardaEngine::Graphics::Animation mCameraRunAnimation;
	float mCameraRunAnimationTime = 0.0f;

	//Light
	NardaEngine::Graphics::DirectionalLight mDirectionalLight;
	NardaEngine::Graphics::StandardEffect mStandardEffect;
	
	//Sky Dome
	NardaEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mSkyDome;

	////Satic environment 
	//NardaEngine::Graphics::RenderGroup mFloatingStage;


	//Particle - Rain
	NardaEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	NardaEngine::Physics::ParticleSystem mRainParticleSystem;

	bool mRainEnable = true;
	NardaEngine::Math::Vector3 mRainoffset = { 0.0f, 8.0f, 0.0f };
	
	//Characters in the proyect 
	NardaEngine::Graphics::RenderGroup mCharacter04;
	NardaEngine::Graphics::Animator mAnimator04;

	NardaEngine::Graphics::RenderGroup mCharacter05;
	NardaEngine::Graphics::Animator mAnimator05;


	// Animation Clips 
	int mRunClip04 = 0;
	int mRunClip05 = 0;

	int mRunToTurnClip04 = 1;
	int mRunToTurnClip05 = 1;

	int mStandingArguingClip04 = 2;
	int mShouttingArgumentsClip05 = 2;

	int mSweepFallClip04 = 3;
	int mPushClip05 = 3;

	int mFallingClip04 = 4;
	int mFloatingClip04 = 5;

	//2D montage / final Memory section 

	NardaEngine::Graphics::Camera mCamera2D;

	NardaEngine::Graphics::SimpleTextureEffect::RenderData mImageQuad;
	std::vector<NardaEngine::Graphics::TextureId> mImageIds;

	float mImageTime = 0.0f;
	float mImageDuration = 0.050f;
	size_t mCurrentImage = 0;

	bool mTogetherMontageStarted = false;

	//Fade quad
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mFadeQuad;
	NardaEngine::Graphics::TextureId mBlackTextureId = 0;
	float mFadeAlpha = 0.0f;

	//Animation speeds (for in place animation control)
	float mAnimationSpeed04 = 0.65f;
	float mAnimationSpeed05 = 0.65f;


	//Movement paths (in place animation control)
	NardaEngine::Math::Vector3 mRunStartPos04 = { -8.0f, 0.0f, -0.4f };
	NardaEngine::Math::Vector3 mRunEndPos04 = { 2.0f, 0.0f, -0.4f };

	NardaEngine::Math::Vector3 mRunStartPos05 = { -8.0f, 0.0f,  0.4f };
	NardaEngine::Math::Vector3 mRunEndPos05 = { 2.0f, 0.0f,  0.4f };

	// Fall path for Character04
	NardaEngine::Math::Vector3 mFallStartPos04 = NardaEngine::Math::Vector3::Zero;
	NardaEngine::Math::Vector3 mFallMidPos04 = { 0.5f, -3.2f, -2.0f };
	NardaEngine::Math::Vector3 mFallEndPos04 = { -0.5f, -9.5f, -4.5f };

	// Debug 
	bool mDrawSkeleton = false;
	float mRunSpeed = 2.2f;

};