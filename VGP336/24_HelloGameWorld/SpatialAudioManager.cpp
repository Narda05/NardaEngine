#include <NardaEngine/Inc/SpEngine.h>
#include "SpatialAudioManager.h"
#include "GameState.h"
#include <chrono>
#include <cmath>


using namespace NardaEngine::Audio;
using namespace NardaEngine::Math;


SpatialAudioManager::~SpatialAudioManager()
{
	Stop();
}
void SpatialAudioManager::RegisterSource(SoundId id, Math::Vector3& position, float maxDistance, float baseVolume)
{
	mSources.push_back({ id, position,maxDistance, baseVolume });
}
void SpatialAudioManager::SetListenerPosition(const Math::Vector3& position) 
{
	std::lock_guard<std::mutex> lock(mMutex);
	mListenerPosition = position;
}
void SpatialAudioManager::Start()
{
	mRunning = true;
	mThread = std::thread(&SpatialAudioManager::AudioThreadLoop, this);
}
void SpatialAudioManager::Stop()
{
	mRunning = false;
	if (mThread.joinable())
	{
		mThread.join();
	}
}
void SpatialAudioManager::AudioThreadLoop()
{
	while (mRunning)
	{
		// Readd the position of the listener like a thread-safe
		Vector3 listenerPos;
		{
			std::lock_guard<std::mutex> lock(mMutex);
			listenerPos = mListenerPosition;
		}

		// Calulate the volumen for distance
		for (const auto& source : mSources)
		{
			Vector3 diff = listenerPos - source.position;
			float distance = std::sqrt(diff.x * diff.x +
									   diff.y * diff.y + 
									   diff.z * diff.z);

			float volume = 1.0f - (distance / source.maxDistance);
			if (volume < 0.0f) volume = 0.0f;
			if (volume > 1.0f) volume = 1.0f;

			SoundEffectManager::Get()->SetVolume(source.id, volume * source.baseVolume);
		}

		//
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}