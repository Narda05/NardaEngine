#include "GameState.h"
#include "math.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;
using namespace NardaEngine::Physics;

void GameState::Initialize() 
{
	mGameWorld.Initialize();

	GameObject* transformGO = mGameWorld.CreateGameObject("Transform");
	transformGO->AddComponent<TransformComponent>();
	transformGO->Initialize();

	GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
	mCameraComponent = cameraGO->AddComponent<CameraComponent>();
	cameraGO->AddComponent<FPSCameraComponent>();
	cameraGO->Initialize();

	GameObject* playerGO = mGameWorld.CreateGameObject("Player");
	TransformComponent* playerTransform = playerGO->AddComponent<TransformComponent>();
	playerTransform->position.x = 2.0f;
	playerGO->Initialize();

	GameObject* playerGO2 = mGameWorld.CreateGameObject("Player2");
	TransformComponent* playerTransform2 = playerGO2->AddComponent<TransformComponent>();
	playerTransform2->position.x = -4.0f; 
	playerGO2->Initialize();

	GameObject* playerGO3 = mGameWorld.CreateGameObject("Player3");
	TransformComponent* playerTransform3 = playerGO3->AddComponent<TransformComponent>();
	playerTransform3->position.x = -6.0f;
	playerGO3->Initialize();

	GameObject* playerGO4 = mGameWorld.CreateGameObject("Player4");
	TransformComponent* playerTransform4 = playerGO4->AddComponent<TransformComponent>();
	playerTransform2->position.x = -8.0f;
	playerGO4->Initialize();

}
void GameState::Terminate() 
{
	mGameWorld.Terminate();
}
void GameState::Update(float deltaTime) 
{
	mGameWorld.Update(deltaTime);
}
void GameState::Render() 
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	ImGui::End();


	SimpleDraw::Render(mCameraComponent->GetCamera());
}




