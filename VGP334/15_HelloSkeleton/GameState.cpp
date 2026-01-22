#include "GameState.h"
#include "math.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//Custom the light configuration
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	mCharacter.Initialize("Character01/Character01.model");
	mCharacter2.Initialize("Character02/Character02.model");
	mCharacter3.Initialize("Character03/Character03.model");
	mCharacter2.transform.position.x = -2.0f;
	mCharacter3.transform.position.x = 2.0f;

	std::filesystem::path effectPath = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(effectPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate()
{
	mCharacter.Terminate();
	mCharacter2.Terminate();
	mCharacter3.Terminate();
	mStandardEffect.Terminate();
	
}
void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);
}
void GameState::Render() 
{
	

	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(mCharacter3.modelId, boneTransforms);
		AnimationUtil::DrawSkeleton(mCharacter3.modelId, boneTransforms);
		SimpleDraw::AddGroundPlane(20.0f, Colors::White);
		SimpleDraw::Render(mCamera);

	}
	else
	{
		SimpleDraw::AddGroundPlane(20.0f, Colors::White);
		SimpleDraw::Render(mCamera);

		mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter3);
		mStandardEffect.End();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);

		}
		ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
	}
	//==================================================
	// 
	//==================================================
	ImGui::PushID("Character03");
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (uint32_t i = 0; i < mCharacter3.renderObjects.size(); ++i)
		{
			Material& material = mCharacter3.renderObjects[i].material;
			std::string renderObjectId = "RenderObject " + std::to_string(i);
			ImGui::PushID(renderObjectId.c_str());
			if (ImGui::CollapsingHeader(renderObjectId.c_str()))
			{
				ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
				ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
				ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
				ImGui::ColorEdit4("Specular#Material", &material.specular.r);
				ImGui::DragFloat("Shininess#Material", &material.shininess, 0.01f, 0.1f, 1000.0f);

			}
			ImGui::PopID();
		}
	}
	//==================================================
	ImGui::PopID();

	ImGui::Checkbox("Draw Skeleton", &mDrawSkeleton);

	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime) 
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{

		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}


