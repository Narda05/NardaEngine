#pragma once

#include <NardaEngine/Inc/SpEngine.h>

class GameState : public NardaEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	
	void UpdateCamera(float deltaTime);

	void OnSpacePressedEvent(const NardaEngine::Core::Event& e);
	
	NardaEngine::Graphics::Camera mCamera;
	NardaEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
	NardaEngine::Physics::ParticleSystem mParticleSystem;
	NardaEngine::Physics::ParticleSystem mFireworkParticle;

	NardaEngine::Core::EventListenerId mSpacePressedListenerId = 0;
	NardaEngine::Core::EventListenerId mEnterPressedListenerId = 0;

	NardaEngine::Graphics::Animation mFireworkAnimation;
	float mFireworkAnimationTime = 0.0f;

	NardaEngine::Audio::SoundId mLaunchSoundId = 0;
	NardaEngine::Audio::SoundId mExplosionSoundId = 0;
};