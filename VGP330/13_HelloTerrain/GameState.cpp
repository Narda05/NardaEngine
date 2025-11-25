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

	//Characters
	mCharacter.Initialize("Character01/Character01.model");
	
	//Terrain Ground
	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw", 20);
	mGround.meshBuffer.Initialize(mTerrain.mesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("terrain/dirt_seamless.jpg");
	mGround.specMadId = TextureManager::Get()->LoadTexture("terrain/grass_2048.jpg");
	
	std::filesystem::path shaderFile = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mCharacter.transform.position = { 0.0f, 0.0f, 0.0f };

}
void GameState::Terminate()
{
	mTerrainEffect.Terminate();	
	mShadowEffect.Terminate();
	mStandardEffect.Terminate();
	mCharacter.Terminate();
	mGround.Terminate();
}

void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);
}

void GameState::Render() 
{
	mShadowEffect.Begin();
		mShadowEffect.Render(mCharacter);
	mShadowEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();

	mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter);
		mStandardEffect.Render(mGround);
	mStandardEffect.End();

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
	ImGui::PushID("Character01");
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (uint32_t i = 0; i < mCharacter.renderObjects.size(); ++i)
		{
			Material& material = mCharacter.renderObjects[i].material;
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
	ImGui::PopID();
	ImGui::DragFloat3("CharacterPosition", &mCharacter.transform.position.x);
	mStandardEffect.DebugUI();
	mShadowEffect.DebugUI();
	mTerrainEffect.DebugUI();
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

	Math::Vector3 cameraPosition = mCamera.GetPosition();
	float height = mTerrain.GetHeight(cameraPosition);
	if(height >= 0.0f)
	{
		
		cameraPosition.y = height + 1.5f;
		mCamera.SetPosition(cameraPosition);

	}

}


