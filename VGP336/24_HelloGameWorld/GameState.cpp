#include "GameState.h"
#include "math.h"
#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"

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
	return nullptr;
}
Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "CustomDebugDrawComponent")
	{
		return gameObject.GetComponent<CustomDebugDrawComponent>();
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


	mGameWorld.LoadLevel(mLevelFile);

	auto* sem = NardaEngine::Audio::SoundEffectManager::Get();
	NardaEngine::Audio::SoundId id1 = sem->Load("explosion.wav");
	NardaEngine::Audio::SoundId id2 = sem->Load("megamanx_storm_eagle.wav");
	NardaEngine::Audio::SoundId id3 = sem->Load("ThunderSFX.wav");

	sem->Play(id1, true);
	sem->Play(id2, true);
	sem->Play(id3, true);

	NardaEngine::Math::Vector3 pos1 = { 10.0f,  0.0f,   0.0f };
	NardaEngine::Math::Vector3 pos2 = { -15.0f, 0.0f,   5.0f };
	NardaEngine::Math::Vector3 pos3 = { 0.0f,   0.0f,  -20.0f };

	//                          Id,Position, MaxDistance
	mSpatialAudio.RegisterSource(id1, pos1, 20.0f);
	mSpatialAudio.RegisterSource(id2, pos2, 30.0f);
	mSpatialAudio.RegisterSource(id3, pos3, 25.0f);

	mSpatialAudio.Start();

}
void GameState::Terminate() 
{
	mSpatialAudio.Stop();
	mGameWorld.Terminate();

}
void GameState::Update(float deltaTime) 
{
	mGameWorld.Update(deltaTime);

	auto* cameraService = mGameWorld.GetService<NardaEngine::CameraService>();
	const auto& cam = cameraService->GetMain();
	NardaEngine::Math::Vector3 camPos = cam.GetPosition();
	mSpatialAudio.SetListenerPosition(camPos);

}
void GameState::Render() 
{
	mGameWorld.Render();
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




