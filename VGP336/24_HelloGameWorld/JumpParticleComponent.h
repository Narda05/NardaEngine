#pragma once
#include <NardaEngine/Inc/SpEngine.h>
#include "CustomTypeIds.h"


class JumpParticleComponent : public NardaEngine::Component
{
public:

	SET_TYPE_ID(CustomComponentId::JumpParticle);

	void Initialize() override;				// set up particle systems + register event listener
	void Terminate() override;				// remove listener + terminate particle systems
	void Update(float deltaTime) override;	// update both particle systems each frame
	void Render();							// NOT a Component override — called manually from GameState::Render()

	static JumpParticleComponent* sInstance;	// GameState uses this since GameWorld has no FindGameObject()

private: 

	void OnJump(const NardaEngine::Core::Event& e);			// callback fired by EventManager on each jump

	NardaEngine::Physics::ParticleSystem mJumpParticles;			// first jump particles
	NardaEngine::Physics::ParticleSystem     mDoubleJumpParticles;	// second jump particles
	NardaEngine::Graphics::ParticleSystemEffect mParticleEffect;	//shared renderer

	NardaEngine::Core::EventListenerId       mJumpListenerId = 0;
	const NardaEngine::TransformComponent* mTransformComponent = nullptr;


};

