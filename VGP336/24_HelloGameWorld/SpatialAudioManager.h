#pragma once

#include <NardaEngine/Inc/SpEngine.h>
#include <vector>
#include <thread> // creates the dedicate audio thread 
#include <mutex> // protects shared data between threads 
#include <atomic> // thread-safe lifecycle flag 

namespace NardaEngine::Audio
{
	// each source has a world position and maximum audiable distance
	struct AudioSource
	{
		SoundId id;
		Math::Vector3 position;
		float maxDistance;
		float baseVolume;
	};

	class SpatialAudioManager
	{
	public:
		SpatialAudioManager() = default;
		~SpatialAudioManager();

		// Register a sound source with its world position and audiable radius
		void RegisterSource(SoundId id, Math::Vector3& position, float maxDistance, float baseVolume = 1.0f);

		// Called from the main thread every frame with the current camera position
		// Uses a mutx to prevent data races with the audio thread
		void SetListenerPosition(const Math::Vector3& position);

		//launches and stops the audio
		void Start();
		void Stop();

	private:
		// Main loop of the audio thread - runs in parallel with the game loop 
		void AudioThreadLoop();
		
		// List of sound sources
		std::vector<AudioSource> mSources;
		// chared camera position (witten by main thread) 
		Math::Vector3 mListenerPosition{};

		// Protects mListenerPosition between threads
		std::mutex mMutex;
		// thread-safe lifecycle flag to control the loop 
		std::atomic<bool> mRunning{ false };
		// Dedicated audio thread
		std::thread mThread;

	};

}
