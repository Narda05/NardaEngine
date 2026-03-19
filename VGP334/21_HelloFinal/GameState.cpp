#include "GameState.h"
#include "math.h"
#include "GameEnvents.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;
using namespace NardaEngine::Physics;
using namespace NardaEngine::Audio;

void GameState::Initialize() 
{
	//Debug camera
	mDebugCamera.SetPosition({ 0.0f, 0.55f, -3.8f });
	mDebugCamera.SetLookAt({ 0.0f, 0.28f, 0.0f });

	//Audio
	SoundEffectManager* sem = SoundEffectManager::Get();
	mIntroMusicId = sem->Load(L"introMusic.wav");
	mSecondPartMusicId = sem->Load(L"secondPartMusic.wav");
	mBeastSfxId = sem->Load(L"beast.wav");
	mDiscussionSfxId = sem->Load(L"conversationFigthSFX.wav");


	//thunders 
	mThunderSfxId = SoundEffectManager::Get()->Load(L"ThunderSFX.wav");

	GameEvents::SubscribeThunder([this]()
		{
			SoundEffectManager::Get()->Play(mThunderSfxId, false);
		});

	//First scene Run Camera 
	mCameraRunLegs.SetPosition({ -6.0f, 0.55f, -3.8f });
	mCameraRunLegs.SetLookAt({ -6.0f, 0.28f, 0.0f });

	//Camera 2 :frontRunningToTurn
	mCameraFrontTalk.SetPosition({ 5.5f, 1.2f, 0.0f });
	mCameraFrontTalk.SetLookAt({ 2.0f, 0.9f, 0.0f });

	//DISCUTION CAMERAS
	//Discution camera - Character 05
	mCameraArgue05.SetPosition({ 2.4f, 1.35f, -1.2f });
	mCameraArgue05.SetLookAt({ mCharacter05.transform.position.x, 1.25f, mCharacter05.transform.position.z });

	//Discution camera - Character 04)
	mCameraArgue04.SetPosition({ 1.5f, 1.3f, 0.2f });
	mCameraArgue04.SetLookAt({ mCharacter04.transform.position.x, 1.25f, mCharacter04.transform.position.z });

	//Discution wide Shot
	mCameraAgueWide.SetPosition({ 6.0f, 1.5f, 0.0f });
	mCameraAgueWide.SetLookAt({ 2.2f, 1.0f, 0.0f });

	//OrbitCamera 
	mCameraArgueOrbit.SetPosition({ 5.5f, 1.3f, 0.0f });
	mCameraArgueOrbit.SetLookAt({ 2.2f, 1.0f, 0.0f });

	//FALL CAMERAS
	//Push wide camera
	mCameraPushWide.SetPosition({ 0.8f, 1.34, -2.0f });
	mCameraPushWide.SetLookAt({ 2.0f, 1.0f, -0.4f });

	//Falling side camer 
	mCameraFallSide.SetPosition({ 4.0f, 1.8f, -2.0f });
	mCameraFallSide.SetLookAt({ 2.0f, 0.5f, -0.4f });

	//Falling top camera
	mCameraFallTop.SetPosition({ 2.0f, 4.0f, -0.4f });
	mCameraFallTop.SetLookAt({ 2.0f, 0.0f, -0.4f });

	//Final floating static camera 
	mCameraFloatingFinal.SetPosition({ 2.0f, -1.5f, -0.5f });
	mCameraFloatingFinal.SetLookAt({   0.2f, -6.0f, -4.5f });

	mMainCamera = &mCameraRunLegs;

	//Camera animation for runningShot 
	mCameraRunAnimation = AnimationBuilder()
		.AddPositionKey({ -6.0f, 0.55f, -3.8f }, 0.0f)
		.AddPositionKey({ -2.0f, 0.55f, -3.8f }, 2.4f)
		.AddPositionKey({  2.0f, 0.55f, -3.8f }, 4.82f)
		.AddPositionKey({  2.5f, 0.75f, -3.2f }, 5.6f)
		.AddPositionKey({  3.0f, 0.95f, -2.6f }, 6.50f)
		.Build();

	// 2D Camera 
	mCamera2D.SetPosition({ 0.0f, 0.0f, -5.0f });
	mCamera2D.SetLookAt({   0.0f, 0.0f, 0.0f });

	//ImageQuad for final Montage 
	MeshPX imageQuadMesh = MeshBuilder::CreateSpriteQuadPX(8.0f, 4.5f);
	mImageQuad.mesh.Initialize(imageQuadMesh);
	mImageQuad.matWorld = Math::Matrix4::Identity;

	std::string fileName = "Aniamtion2d/scene";
	std::string extension = ".jpg";

	//for (std::size_t i = 1; i <= 118; ++i)
	//{
	//	std::string number = std::to_string(i);
	//	number = std::string(5 - number.size(), '0') + number;
	//	mImageIds.push_back(TextureManager::Get()->LoadTexture(fileName + number + extension));
	//}
	int indexValue = 1;
	for (std::size_t i = 0; i < 182; ++i)
	{
		std::string number = std::to_string(indexValue);
		number = std::string(5 - number.size(), '0') + number;
		mImageIds.push_back(TextureManager::Get()->LoadTexture(fileName + number + extension));
		indexValue += 5;
	}

	if (!mImageIds.empty())
	{
		mCurrentImage = 0;
		mImageQuad.textureId = mImageIds[mCurrentImage];
		mImageTime = 0.0f;
		mImageDuration = 0.1f;
	}

	//fade 
	MeshPX fadeQuadMesh = MeshBuilder::CreateSpriteQuadPX(8.0f, 4.5f);
	mFadeQuad.mesh.Initialize(fadeQuadMesh);
	mFadeQuad.matWorld = Math::Matrix4::Identity;
	mBlackTextureId = TextureManager::Get()->LoadTexture("Images/black.jpg");
	mFadeQuad.textureId = mBlackTextureId;


	//Light
	mDirectionalLight.direction = Math::Normalize({ 0.036f, -0.965f, 0.261f });
	mDirectionalLight.ambient =  { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.diffuse =  { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.specular = { 0.0f, 0.0f, 0.0f, 1.0f };

	//Skydome
	mSimpleTextureEffect.Initialize();

	MeshPX skySphere = MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
	mSkyDome.mesh.Initialize(skySphere);
	mSkyDome.textureId = TextureManager::Get()->LoadTexture("space.jpg");
	mSkyDome.matWorld = Math::Matrix4::Identity;


	////Static environment 
	//mFloatingStage.Initialize("Island/Flooting_Stage.model");
	//mFloatingStage.transform.position = { 0.0f, -1.0f, 0.0f };
	//mFloatingStage.transform.scale = {1.0f, 1.0f, 1.0f };
	//mFloatingStage.transform.rotation = Math::Quaternion::Identity;
	


	//Characters + Animations
	ModelManager* mm = ModelManager::Get();
	//character 04
	mCharacter04.Initialize("Character04/Character04.model");
	mCharacter04.animator = &mAnimator04;
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/FastRun04.animset");
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/RunningToTurn04.animset");
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/StandingArguing04.animset");
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/SweepFall04.animset");
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/Falling04.animset");
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/Floating04.animset");
	mAnimator04.Initialize(mCharacter04.modelId);
	if (mAnimator04.GetAnimationCount() > 0)
	{
		mCharacter04.transform.position = mRunStartPos04;
		mCharacter04.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
		mCharacter04.transform.scale = { 0.67f, 0.67f, 0.67f };
		mAnimator04.PlayAnimation(mRunClip04, true);
	}

	for (uint32_t i = 0; i < mCharacter04.renderObjects.size(); ++i)
	{
		mCharacter04.renderObjects[i].material.ambient =  { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter04.renderObjects[i].material.diffuse =  { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter04.renderObjects[i].material.specular = { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter04.renderObjects[i].material.emissive = { 0.4f, 0.4f, 0.4f, 0.4f };
	}
	
	//character 05
	mCharacter05.Initialize("Character05/Character05.model");
	mCharacter05.animator = &mAnimator05;
	mm->AddAnimation(mCharacter05.modelId, L"../../Assets/Models/Character05/Animation/FastRun05.animset");
	mm->AddAnimation(mCharacter05.modelId, L"../../Assets/Models/Character05/Animation/RunningToTurn05.animset");
	mm->AddAnimation(mCharacter05.modelId, L"../../Assets/Models/Character05/Animation/ShoutingArguments05.animset");
	mm->AddAnimation(mCharacter05.modelId, L"../../Assets/Models/Character05/Animation/Push05.animset");
	mAnimator05.Initialize(mCharacter05.modelId);
	if (mAnimator05.GetAnimationCount() > 0)
	{
		mCharacter05.transform.position = mRunStartPos05;
		mCharacter05.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
		mCharacter05.transform.scale = { 1.0f, 1.0f, 1.0f };
		mAnimator05.PlayAnimation(mRunClip05, true);
	}

	for (uint32_t i = 0; i < mCharacter05.renderObjects.size(); ++i)
	{
		mCharacter05.renderObjects[i].material.ambient = { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter05.renderObjects[i].material.diffuse = { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter05.renderObjects[i].material.specular = { 0.4f, 0.4f, 0.4f, 0.4f };
		mCharacter05.renderObjects[i].material.emissive = { 0.4f, 0.4f, 0.4f, 0.4f };
	}

	// Initial positions for the running shot
	mCharacter04.transform.position = mRunStartPos04;
	mCharacter05.transform.position = mRunStartPos05;

	//Effect
	std::filesystem::path effectPath = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(effectPath);
	mStandardEffect.SetCamera(mDebugCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	
	//Particle SYStem
	mParticleSystemEffect.Initialize();
	mParticleSystemEffect.SetCamera(*mMainCamera);

	ParticleSystemInfo rainInfo;
	rainInfo.textureId = TextureManager::Get()->LoadTexture("Images/bullet1.png");
	rainInfo.maxParticles = 5000;
	rainInfo.particlesPerEmit = { 40, 70 };
	rainInfo.delay = 0.0f;
	rainInfo.lifeTime = FLT_MAX;
	rainInfo.timeBetweenEmit = { 0.01f, 0.02f };
	rainInfo.spawnAngle = { -55.0f, 55.0f };
	rainInfo.spawnSpeed = { 14.0f, 22.0f };
	rainInfo.particleLifeTime = { 1.0f, 2.0f };
	rainInfo.spawnDirection = -Math::Vector3::YAxis;
	rainInfo.spawnPosition = Math::Vector3::Zero;
	rainInfo.startScale = { {0.02f, 0.22f, 0.02f}, {0.04f, 0.35f, 0.04f} };
	rainInfo.endScale	= { {0.01f, 0.12f, 0.01f}, {0.03f, 0.14f, 0.03f} };
	rainInfo.startColor = { Graphics::Colors::LightBlue, Graphics::Colors::White };
	rainInfo.endColor	= { Graphics::Colors::LightBlue, Graphics::Colors::LightGray };

	mRainParticleSystem.Initialize(rainInfo);



}
void GameState::Terminate() 
{

	SoundEffectManager* sm = SoundEffectManager::Get();
	sm->Stop(mIntroMusicId);
	sm->Stop(mSecondPartMusicId);
	sm->Stop(mBeastSfxId);
	sm->Stop(mDiscussionSfxId);

	mSkyDome.mesh.Terminate();
	TextureManager::Get()->RealeaseTexture(mSkyDome.textureId);
	mSimpleTextureEffect.Terminate();

	//Particles 
	mRainParticleSystem.Terminate();
	mParticleSystemEffect.Terminate();

	//mFloatingStage.Terminate();
	mCharacter04.Terminate();
	mCharacter05.Terminate();
	mStandardEffect.Terminate();

	//2D 
	for (auto id : mImageIds)
	{
		TextureManager::Get()->RealeaseTexture(id);
	}
	mImageIds.clear();

	mImageQuad.mesh.Terminate();
	mFadeQuad.mesh.Terminate();

	TextureManager::Get()->RealeaseTexture(mBlackTextureId);

}
void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);

	float dt = mIsPlaying ? deltaTime : 0.0f;
	float prevTime = mSceneTime;

	// Scene timer
	if (mIsPlaying)
	{
		mSceneTime += deltaTime;
		if (mSceneTime > mSceneDuration)
		{
			mSceneTime = mSceneDuration;
		}
	}

	mCameraRunAnimationTime = mSceneTime;

	//Scene logic
	UpdateScene(dt);


	//Update animators
	if (mAnimator04.GetAnimationCount() > 0)
	{
		mAnimator04.Update(dt * mAnimationSpeed04);
	}

	if (mAnimator05.GetAnimationCount() > 0)
	{
		mAnimator05.Update(dt * mAnimationSpeed05);
	}

	//Audio logic
	InputSystem* input = InputSystem::Get();
	if (input->IsKeyPressed(KeyCode::SPACE))
	{
		GameEvents::BroadcastThunder();
	}

	const Camera& activeCamera = mUseDebugCamera ? mDebugCamera : *mMainCamera;
	mParticleSystemEffect.SetCamera(activeCamera);

	if (mRainEnable)
	{
		NardaEngine::Math::Vector3 rainPos = { 2.0f , 10.0f, -1.5f };
		mRainParticleSystem.SetPosition(rainPos);
		mRainParticleSystem.Update(deltaTime);
	}
	


}
void GameState::Render() 
{

	if (!mUseDebugCamera)
	{
		if (mMainCamera == &mCameraRunLegs)
		{
			Math::Vector3 camPos = mCameraRunAnimation.GetTransfor(mCameraRunAnimationTime).position;
			mCameraRunLegs.SetPosition(camPos);

			// Keep camera looking at the characters' leg area
			Math::Vector3 centerPos =
				(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;

			float lookAtHeight = (mSceneTime < mRunSectionEnd) ? 0.28f : 0.45f;
			mCameraRunLegs.SetLookAt({ centerPos.x,0.9f , centerPos.z });
		}

	}

	const Camera& activeCamera = mUseDebugCamera ? mDebugCamera : *mMainCamera;

	//2D Final Montage in render
	if (mSceneTime >= mTogetherSectionStart)
	{
		mSimpleTextureEffect.SetCamera(mCamera2D);
		mSimpleTextureEffect.Begin();

		if (mSceneTime < mFadeSectionStart)
		{
			if (!mImageIds.empty())
			{
				mSimpleTextureEffect.Render(mImageQuad);

			}
		}
		else
		{
			mSimpleTextureEffect.Render(mFadeQuad);
		}

		mSimpleTextureEffect.End();
		return;
		
	}


	//SkyDome
	mSimpleTextureEffect.SetCamera(activeCamera);
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkyDome);
	mSimpleTextureEffect.End();


	mStandardEffect.SetCamera(activeCamera);

	mParticleSystemEffect.SetCamera(activeCamera);

	//Skeleton
	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;

		AnimationUtil::ComputeBoneTransforms(mCharacter04.modelId, boneTransforms, &mAnimator04);
		AnimationUtil::DrawSkeleton(mCharacter04.modelId, boneTransforms);

		AnimationUtil::ComputeBoneTransforms(mCharacter05.modelId, boneTransforms, &mAnimator05);
		AnimationUtil::DrawSkeleton(mCharacter05.modelId, boneTransforms);
		
		SimpleDraw::AddGroundPlane(20.0f, Colors::White);
		SimpleDraw::Render(activeCamera);

	}
	else
	{
		SimpleDraw::AddGroundPlane(20.0f, Colors::White);
		SimpleDraw::Render(activeCamera);

		mStandardEffect.Begin();
		//mStandardEffect.Render(mFloatingStage);
		mStandardEffect.Render(mCharacter04);
		mStandardEffect.Render(mCharacter05);
		mStandardEffect.End();

		if (mRainEnable)
		{
			mParticleSystemEffect.Begin();
			mRainParticleSystem.Render(mParticleSystemEffect);
			mParticleSystemEffect.End();
		}

	}

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Scene Time: %.2f / %.2f", mSceneTime, mSceneDuration);
	ImGui::ProgressBar(mSceneTime / mSceneDuration, ImVec2(220.0f, 20.0f));

	ImGui::Checkbox("Play Scene", &mIsPlaying);
	ImGui::Checkbox("Use Debug Camera", &mUseDebugCamera);
	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);

	ImGui::Separator();
	ImGui::Text("Running Section");
	ImGui::DragFloat("Run Speed", &mRunSpeed, 0.1f, 0.0f, 20.0f);
	ImGui::DragFloat("Anim Speed A", &mAnimationSpeed04, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Anim Speed B", &mAnimationSpeed05, 0.1f, 0.0f, 10.0f);

	ImGui::Separator();
	ImGui::Text("Character 04 pos:  %.2f, %.2f, %.2f ",
		mCharacter04.transform.position.x,
		mCharacter04.transform.position.y,
		mCharacter04.transform.position.z);

	ImGui::Text("Character 05 pos:  %.2f, %.2f, %.2f ",
		mCharacter05.transform.position.x,
		mCharacter05.transform.position.y,
		mCharacter05.transform.position.z);
	ImGui::Separator();
	ImGui::Checkbox("Enable Rain", &mRainEnable);
	ImGui::DragFloat3("Rain Offset", &mRainoffset.x, 0.1f, -10.0f, 10.0f);
	

	ImGui::DragFloat3("Ligth", &mDirectionalLight.direction.x, 0.1f,360.0f, 0.1f);
	mRainParticleSystem.DebugUI();
	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateScene(float deltaTime)
{
	if (mSceneTime < mRunSectionEnd)
	{
		UpdateRunningSection(deltaTime);
	}
	else if (mSceneTime < mTurnSectionEnd)
	{
		UpdateTalkingSection(deltaTime);
	}
	else if (mSceneTime < mDiscussionSectionEnd)
	{
		UpdateDiscussionSection(deltaTime);
	}
	else if (mSceneTime < mFallSectionEnd)
	{
		UpdateFallSection(deltaTime);
	}
	else if (mSceneTime < mTogetherSectionEnd)
	{
		UpdateTogetherSection(deltaTime);
	}
	else
	{
		UpdateFadeSection(deltaTime);
	}
}


void GameState::UpdateRunningSection(float deltaTime)
{
	if (!mIsRunningStarted)
	{
		mIsRunningStarted = true;
		mMainCamera = &mCameraRunLegs;

		if (mAnimator04.GetAnimationCount() > mRunClip04)
		{
			mAnimator04.PlayAnimation(mRunClip04, true);
		}

		if (mAnimator05.GetAnimationCount() > mRunClip05)
		{
			mAnimator05.PlayAnimation(mRunClip05, true);
		}

		mAnimationSpeed04 = 0.65f;
		mAnimationSpeed05 = 0.65f;

		mCharacter04.transform.position = mRunStartPos04;
		mCharacter05.transform.position = mRunStartPos05;

		mCharacter04.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
		mCharacter05.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);

		if(!mIntroMusicStarted)
		{
			mIntroMusicStarted = true;
			SoundEffectManager::Get()->Play(mIntroMusicId, false);
		}
	}

	//Normalize Time 
	float sectionDuration = mRunSectionEnd - mRunSectionStart;
	float t = (mSceneTime - mRunSectionStart) / sectionDuration;

	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;



	//Move characters forward using Lerp
	mCharacter04.transform.position = NardaEngine::Math::Lerp(mRunStartPos04, mRunEndPos04, t);
	mCharacter05.transform.position = NardaEngine::Math::Lerp(mRunStartPos05, mRunEndPos05, t);


}

void GameState::UpdateTalkingSection(float deltaTime)
{
	if (!mRunToTurnStarted)
	{
		mRunToTurnStarted = true;

		mMainCamera = &mCameraFrontTalk;

		if(mAnimator04.GetAnimationCount() > mRunToTurnClip04)
		{
			mAnimator04.PlayAnimation(mRunToTurnClip04, false);
		}

		if (mAnimator05.GetAnimationCount() > mRunToTurnClip05)
		{
			mAnimator05.PlayAnimation(mRunToTurnClip05, false);
		}

		mAnimationSpeed04 = 1.0f;
		mAnimationSpeed05 = 1.0f;

		if(!mBeastSfxStarted)
		{
			mBeastSfxStarted = true;
			SoundEffectManager::Get()->Play(mBeastSfxId, false);
		}

	}

	// If RunningToTurn should stay in the same place, keep these:
	mCharacter04.transform.position = mRunEndPos04;
	mCharacter05.transform.position = mRunEndPos05;

	//Position
	mCharacter04.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
	mCharacter05.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);

	Math::Vector3 centerPos =
		(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;

	mCameraFrontTalk.SetPosition({ centerPos.x - 3.5f, 1.4f, centerPos.z });
	mCameraFrontTalk.SetLookAt({ centerPos.x , 1.3f, centerPos.z });
}

void GameState::UpdateDiscussionSection(float deltaTime)
{
	if (!mDiscussionStarted)
	{
		mDiscussionStarted = true;

		//Play Animation 
		if (mAnimator04.GetAnimationCount() > mStandingArguingClip04)
		{
			mAnimator04.PlayAnimation(mStandingArguingClip04, true);
		}

		if (mAnimator05.GetAnimationCount() > mShouttingArgumentsClip05)
		{
			mAnimator05.PlayAnimation(mShouttingArgumentsClip05, true);
		}

		if(!mDiscussionSfxStarted)
		{
			mDiscussionSfxStarted = true;
			SoundEffectManager::Get()->Play(mDiscussionSfxId, false);
		}
		mAnimationSpeed04 = 1.0f;
		mAnimationSpeed05 = 1.0f;

	}

	//keeping the stay in the same palce until the end of the discussion section, then we can save the positions for the next section (falling)
	mCharacter04.transform.position = mRunEndPos04;
	mCharacter05.transform.position = mRunEndPos05;

	// Make them look at each other from those final positions
	Math::Vector3 dir04To05 = mCharacter05.transform.position - mCharacter04.transform.position;
	Math::Vector3 dir05To04 = mCharacter04.transform.position - mCharacter05.transform.position;

	float yaw04 = atan2f(dir04To05.x, dir04To05.z) + Math::Constants::Pi; // Add 180 degrees to make them face each other
	float yaw05 = atan2f(dir05To04.x, dir05To04.z) + Math::Constants::Pi; // Add 180 degrees to make them face each other

	mCharacter04.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, yaw04);
	mCharacter05.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, yaw05);

	Math::Vector3 centerPos =
		(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;

	//Camera order for discussion 
	if (mSceneTime < 9.0f)
	{
		mMainCamera = &mCameraArgue05;
		mCameraArgue05.SetLookAt({ mCharacter05.transform.position.x, 1.0f, mCharacter05.transform.position.z });
	}
	else if (mSceneTime < 11.5f)
	{
		mMainCamera = &mCameraArgue04;
		mCameraArgue04.SetLookAt({ mCharacter04.transform.position.x, 1.0f, mCharacter04.transform.position.z });
	}
	else if (mSceneTime < 14.0f)
	{
		mMainCamera = &mCameraAgueWide;
		mCameraAgueWide.SetLookAt({ centerPos.x, 1.0f, centerPos.z });
	}
	else
	{
		mMainCamera = &mCameraArgueOrbit;

		float orbitRadius = 3.0f;
		float orbitSpeed = 1.0f; // Radians per seconds
		float angle = (mSceneTime - 14.0f) * orbitSpeed;

		Math::Vector3 orbitPos =
		{
			centerPos.x + std::cos(angle) * orbitRadius,
			1.4f,
			centerPos.z + std::sin(angle) * orbitRadius
		};

		mCameraArgueOrbit.SetPosition(orbitPos);
		mCameraArgueOrbit.SetLookAt({ centerPos.x, 1.0f, centerPos.z });

	}
}

void GameState::UpdateFallSection(float deltaTime)
{
	if (!mPushStarted)
	{
		mPushStarted = true;
		mFallingStarted = false;
		mFloatingStarted = false;


		mMainCamera = &mCameraPushWide;

		if (mAnimator05.GetAnimationCount() > mPushClip05)
		{
			mAnimator05.PlayAnimation(mPushClip05, false);
		}

		if (mAnimator04.GetAnimationCount() > mSweepFallClip04)
		{
			mAnimator04.PlayAnimation(mSweepFallClip04, false);
		}

		mAnimationSpeed04 = 1.0f;
		mAnimationSpeed05 = 1.0f;
	}

	//Part1 :push
	if (mSceneTime < 18.2f)
	{
		mMainCamera = &mCameraPushWide;

		// Keep positions from the end of discussion
		mCharacter04.transform.position = mRunEndPos04;
		mCharacter05.transform.position = mRunEndPos05;

		Math::Vector3 dir04To05 = mCharacter05.transform.position - mCharacter04.transform.position;
		Math::Vector3 dir05To04 = mCharacter04.transform.position - mCharacter05.transform.position;

		float yaw04 = atan2f(dir04To05.x, dir04To05.z) + Math::Constants::Pi;
		float yaw05 = atan2f(dir05To04.x, dir05To04.z) + Math::Constants::Pi;

		mCharacter04.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, yaw04);

		mCharacter05.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, yaw05);

		Math::Vector3 centerPos =
			(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;

		mCameraPushWide.SetLookAt({ centerPos.x, 1.0f, centerPos.z });

		// Save where the actual fall starts
		mFallStartPos04 = mCharacter04.transform.position;
		return;
	}

	//PART2 FOR FALLING
	if (mSceneTime < 20.0f)
	{
		if (!mFallingStarted)
		{
			mFallingStarted = true;
			mMainCamera = &mCameraFallSide;

			if (mAnimator04.GetAnimationCount() > mFallingClip04)
			{
				mAnimator04.PlayAnimation(mFallingClip04, true);
			}

			if (!mSecondPartMusicStarted)
			{
				SoundEffectManager::Get()->Play(mSecondPartMusicId, false);
				mSecondPartMusicStarted = true;
			}

		}

		// Character05 stays up there
		mCharacter05.transform.position = mRunEndPos05;

		float t = (mSceneTime - 18.2f) / (20.0f - 18.2f);
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		mCharacter04.transform.position = Math::Lerp(mFallStartPos04, mFallMidPos04, t);

		// slight tilt while falling
		mCharacter04.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, 180.0f * Math::Constants::DegToRad);

		mCameraFallSide.SetLookAt({
			mCharacter04.transform.position.x,
			mCharacter04.transform.position.y + 1.2f,
			mCharacter04.transform.position.z
			});
		return;
	}

	//PART3 for Vertical Falling with FLOATING
	if (mSceneTime >= 20.0f)
	{
		if (!mFallingStarted)
		{
			mFallingStarted = true;

			if (mAnimator04.GetAnimationCount() > mFallingClip04)
			{
				mAnimator04.PlayAnimation(mFallingClip04, true);
			}


		}

		// Character05 stays up there
		mCharacter05.transform.position = mRunEndPos05;

		//Move character04 final floating pose
		float t = (mSceneTime - 20.0f) / (mFallSectionEnd - 20.0f);
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		mCharacter04.transform.position = Math::Lerp(mFallStartPos04, mFallMidPos04, t);

		// Fina floating pose 
		mCharacter04.transform.rotation =
			Math::Quaternion::CreateFromAxisAngle(Math::Vector3::ZAxis, 180.0f * Math::Constants::DegToRad);



		//Define the cut to change the camera 
		if (t < 0.85f)
		{
			mMainCamera = &mCameraFallTop;

			mCameraFallTop.SetPosition({
			mCharacter04.transform.position.x + 1.2f,
			mCharacter04.transform.position.y + 4.0f,
			mCharacter04.transform.position.z + 1.8f
			});

			mCameraFallTop.SetLookAt({
				mCharacter04.transform.position.x,
				mCharacter04.transform.position.y + 1.0f,
				mCharacter04.transform.position.z
				});
		}
		else
		{
			//Static finalcamera
			if (!mFloatingFinalCameraStarted)
			{
				mFloatingFinalCameraStarted = true;
				mMainCamera = &mCameraFloatingFinal;
			}

			mMainCamera = &mCameraFloatingFinal;
		}
		
	}

}

void GameState::UpdateTogetherSection(float deltaTime)
{
	if (!mTogetherStarted)
	{
		mTogetherStarted = true;
		mMainCamera = &mCamera2D;

		mImageTime = 0.0f;
		mCurrentImage = 0;

		if (!mImageIds.empty())
		{
			mImageQuad.textureId = mImageIds[mCurrentImage];
		}
	}

	if (!mImageIds.empty())
	{
		mImageTime += deltaTime;

		while (mImageTime >= mImageDuration)
		{
			mImageTime -= mImageDuration;

			if (mCurrentImage + 1 < mImageIds.size())
			{
				++mCurrentImage;
				mImageQuad.textureId = mImageIds[mCurrentImage];
			}
		}
	}
}

void GameState::UpdateFadeSection(float deltaTime)
{
	if (!mFadeStarted)
	{
		mFadeStarted = true;
	}
}

void GameState::UpdateCamera(float deltaTime) 
{
	if (!mUseDebugCamera)
	{
		return;
	}

	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 3.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mDebugCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mDebugCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mDebugCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mDebugCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mDebugCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mDebugCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{

		mDebugCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mDebugCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}

}


