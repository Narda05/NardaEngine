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

	//First scene Run Camera 
	mCameraRunLegs.SetPosition({ -6.0f, 0.55f, -3.8f });
	mCameraRunLegs.SetLookAt({ -6.0f, 0.28f, 0.0f });

	mCameraRunAnimation = AnimationBuilder()
		.AddPositionKey({ -6.0f, 0.55f, -3.8f }, 0.0f)
		.AddPositionKey({ -2.0f, 0.55f, -3.8f }, 2.4f)
		.AddPositionKey({ 2.0f, 0.55f, -3.8f }, 4.82f)
		.Build();

	//Light
	mDirectionalLight.direction = Math::Normalize({ 1.0f, 1.0f, 1.0f });
	mDirectionalLight.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 0.8f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };


	//Characters
	ModelManager* mm = ModelManager::Get();

	//character 04
	mCharacter04.Initialize("Character04/Character04.model");
	mCharacter04.animator = &mAnimator04;
	mm->AddAnimation(mCharacter04.modelId, L"../../Assets/Models/Character04/Animation/FastRun04.animset");
	mAnimator04.Initialize(mCharacter04.modelId);
	if (mAnimator04.GetAnimationCount() > 0)
	{
		mCharacter04.transform.position = mRunStartPos04;
		mAnimator04.PlayAnimation(mRunClip04, true);
	}
	
	//character 05
	mCharacter05.Initialize("Character05/Character05.model");
	mCharacter05.animator = &mAnimator05;
	mm->AddAnimation(mCharacter05.modelId, L"../../Assets/Models/Character05/Animation/FastRun05.animset");
	mAnimator05.Initialize(mCharacter05.modelId);
	if (mAnimator05.GetAnimationCount() > 0)
	{
		mCharacter05.transform.position = mRunStartPos05;
		mAnimator05.PlayAnimation(mRunClip05, true);
	}

	// Initial positions for the running shot
	mCharacter04.transform.position = mRunStartPos04;
	mCharacter05.transform.position = mRunStartPos05;

	mCharacter04.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
	mCharacter05.transform.rotation =
		Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);


	std::filesystem::path effectPath = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(effectPath);
	mStandardEffect.SetCamera(mDebugCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	

}
void GameState::Terminate() 
{
	mCharacter04.Terminate();
	mCharacter05.Terminate();
	mStandardEffect.Terminate();

}
void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);

	float dt = mIsPlaying ? deltaTime : 0.0f;

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

}
void GameState::Render() 
{

	if (!mUseDebugCamera && mMainCamera == &mCameraRunLegs)
	{
		Math::Vector3 camPos = mCameraRunAnimation.GetTransfor(mCameraRunAnimationTime).position;
		mCameraRunLegs.SetPosition(camPos);

		// Keep camera looking at the characters' leg area
		Math::Vector3 centerPos =
			(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;

		mCameraRunLegs.SetLookAt({ centerPos.x, 0.28f, centerPos.z });
	}

	const Camera& activeCamera = mUseDebugCamera ? mDebugCamera : *mMainCamera;

	mStandardEffect.SetCamera(activeCamera);

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
		mStandardEffect.Render(mCharacter04);
		mStandardEffect.Render(mCharacter05);
		mStandardEffect.End();

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
	
	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateScene(float deltaTime)
{
	if (mSceneTime < 4.82f)
	{
		UpdateRunningSection(deltaTime);
	}
	else if (mSceneTime < 17.0f)
	{
		UpdateTalkingSection(deltaTime);
	}
	else if (mSceneTime < 22.0f)
	{
		UpdateFallSection(deltaTime);
	}
	else if (mSceneTime < 46.0f)
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

		mAnimationSpeed04 = 0.65f;
		mAnimationSpeed05 = 0.65f;

		mCharacter04.transform.position = mRunStartPos04;
		mCharacter05.transform.position = mRunStartPos05;
	}

	//Normalize Time 
	float sectionDuration = mRunSectionEnd - mRunSectionStart;
	float t = (mSceneTime - mRunSectionStart) / sectionDuration;

	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;



	//Move characters forward using Lerp
	mCharacter04.transform.position = NardaEngine::Math::Lerp(mRunStartPos04, mRunEndPos04, t);
	mCharacter05.transform.position = NardaEngine::Math::Lerp(mRunStartPos05, mRunEndPos05, t);

	//Camera Follow 
	Math::Vector3 centerPos = 
		(mCharacter04.transform.position + mCharacter05.transform.position) * 0.5f;
	NardaEngine::Math::Vector3 desiredCamPos =
	{
		centerPos.x,
		0.55f,
		-3.8f
	};

	NardaEngine::Math::Vector3 desiredLookAt =
	{
		centerPos.x,
		0.28f,
		centerPos.z
	};
	

	NardaEngine::Math::Vector3 currentPos = mCameraRunLegs.GetPosition();
	NardaEngine::Math::Vector3 newPos = NardaEngine::Math::Lerp(currentPos, desiredCamPos, 0.0f);

	mCameraRunLegs.SetPosition(newPos);
	mCameraRunLegs.SetLookAt(desiredLookAt);
}

void GameState::UpdateTalkingSection(float deltaTime)
{
	if (!mTalkingStarted)
	{
		mTalkingStarted = true;

		mAnimationSpeed04 = 0.0f;
		mAnimationSpeed05 = 0.0f;
	}

}

void GameState::UpdateFallSection(float deltaTime)
{
	if (!mFallStarted)
	{
		mFallStarted = true;
	}
}

void GameState::UpdateTogetherSection(float deltaTime)
{
	if (!mTogetherStarted)
	{
		mTogetherStarted = true;
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
	const float turnSpeed = 2.0f;

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


