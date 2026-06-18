#include "GameState.h"
#include "math.h"
#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"
#include "JumpParticleComponent.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;
using namespace NardaEngine::Physics;

Service* MakeCustomService(const std::string& serviceName, GameWorld& gameWorld)
{
	if (serviceName == "CustomDebugDrawService")
	{
		return gameWorld.AddService<CustomDebugDrawService>();
	}

	// add another 
	// add next..
	return nullptr;
		
}

Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "CustomDebugDrawComponent")
	{
		return gameObject.AddComponent<CustomDebugDrawComponent>();
	}
	if (componentName == "JumpParticleComponent")
	{
		return gameObject.AddComponent<JumpParticleComponent>();
	}
	return nullptr;
}
Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "CustomDebugDrawComponent")
	{
		return gameObject.GetComponent<CustomDebugDrawComponent>();
	}
	if (componentName == "JumpParticleComponent")
	{
		return gameObject.GetComponent<JumpParticleComponent>();
	}
	return nullptr;
}

void GameState::Initialize() 
{
	mLevelFile = L"../../Assets/Templates/Levels/level.json";
	
	// set a callback to try make a custom service (any serice that is NOT part of the engine and unique to the project)
	GameWorld::SetCustomService(MakeCustomService);
	//sets callbacks to try make/get a custom component (any component that is NOT part of the engine)
	GameObjectFactory::SetCustomMake(MakeCustomComponent);
	GameObjectFactory::SetCustomGet(GetCustomComponent);

	//Skydome 
	mSimpleTextureEffect.Initialize();
	NardaEngine::Graphics::MeshPX skySphere = NardaEngine::Graphics::MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
	mSkySphere.mesh.Initialize(skySphere);
	mSkySphere.textureId = NardaEngine::Graphics::TextureManager::Get()->LoadTexture("minerals.png");


	mGameWorld.LoadLevel(mLevelFile);

}
void GameState::Terminate() 
{
	mSkySphere.mesh.Terminate();
	NardaEngine::Graphics::TextureManager::Get()->RealeaseTexture(mSkySphere.textureId);
	mSimpleTextureEffect.Terminate();

	mGameWorld.Terminate();
}
void GameState::Update(float deltaTime) 
{
	mGameWorld.Update(deltaTime);
}
void GameState::Render() 
{
	// Render skydome first
	auto* cameraService = mGameWorld.GetService<NardaEngine::CameraService>();
	mSimpleTextureEffect.SetCamera(cameraService->GetMain());
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.End();

	mGameWorld.Render();

	if (JumpParticleComponent::sInstance)
	{
		JumpParticleComponent::sInstance->Render();
	}

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	if (ImGui::Button("ReloadLevel"))
	{
		mGameWorld.Terminate();
		mGameWorld.LoadLevel(mLevelFile);
	}
	ImGui::End();
}




