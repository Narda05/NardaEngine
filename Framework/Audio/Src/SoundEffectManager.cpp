#include "Precompiled.h"
#include "SoundEffectManager.h"
#include "AudioSystem.h"

using namespace DirectX; 
using namespace NardaEngine;
using namespace NardaEngine::Audio;

namespace
{
	std::unique_ptr<SoundEffectManager> sSoundEffectManager;
}

void SoundEffectManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sSoundEffectManager == nullptr, "SoundEffectManager: is already initialized");
	sSoundEffectManager = std::make_unique<SoundEffectManager>();
	sSoundEffectManager->SetRootPath(root);
}
void SoundEffectManager::StaticTerminate()
{
	if (sSoundEffectManager != nullptr)
	{
		sSoundEffectManager->Clear();
		sSoundEffectManager.reset();
	}
}
SoundEffectManager* SoundEffectManager::Get()
{
	ASSERT(sSoundEffectManager != nullptr, "SoundEffectManager: is not initialized");
	return sSoundEffectManager.get();
}
SoundEffectManager::~SoundEffectManager()
{
	ASSERT(mInventory.empty(), "SoundEffectManager: is not terminated");
}
void SoundEffectManager::SetRootPath(const std::filesystem::path& root)
{
	mRoot = root;
}

SoundId SoundEffectManager::Load(const std::filesystem::path& fileName)
{
	std::filesystem::path fullPath = mRoot / fileName;
	std::size_t soundId = std::filesystem::hash_value(fullPath);
	auto [iter, success] = mInventory.insert({ soundId, nullptr });
	if (success)
	{
		AudioSystem* as = AudioSystem::Get();
		auto& soundEventPtr = iter->second;
		soundEventPtr = std::make_unique<Entry>();
		soundEventPtr->effect = std::make_unique<SoundEffect>(as->mAudioEngine, fullPath.wstring().c_str());
		soundEventPtr->instance = soundEventPtr->effect->CreateInstance();

		////
		//soundEventPtr->instance->SetVolume;

	// in game state add a ¨3d audio manager (just in game state)
	// make it threaded
	// add several audioids and positions for each
		// monitor where camera is and adjust sound volume
		// SoundEffecManager::Get()->SetVolume(id, volume)
	}
	return soundId;
}
void SoundEffectManager::Clear()
{
	AudioSystem::Get()->Suspend();
	for (auto& entry : mInventory)
	{
		if (entry.second)
		{
			entry.second->instance->Stop();
			entry.second->instance.reset();
			entry.second->effect.reset();
			entry.second.reset();
		}
	}
	mInventory.clear();
}
void SoundEffectManager::Play(SoundId id, bool loop)
{
	auto iter = mInventory.find(id);
	if ( iter != mInventory.end())
	{
		iter->second->instance->Stop();
		iter->second->instance->Play(loop);
	}
}
void SoundEffectManager::Stop(SoundId id)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->instance->Stop();
	}
}

void SoundEffectManager::SetVolume(SoundId id, float volume)
{
	// Search for the sound in the inventory by its ID
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		//Directly set volume on the directX SoundEffectInstance
		// volume ranges from 0.0 (silence) to 1.0 (maximum)
		iter->second->instance->SetVolume(volume);
	}
}
