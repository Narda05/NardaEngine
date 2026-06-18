#pragma once

#include <NardaEngine/Inc/SpEngine.h>
#include "JumpParticleComponent.h"

class GameState : public NardaEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	std::filesystem::path mLevelFile;
	NardaEngine::GameWorld mGameWorld;
	JumpParticleComponent* mJumpParticleComponent = nullptr;

	NardaEngine::Core::EventListenerId mJumpListenerId  = 0;

	//Skydome
	NardaEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mSkySphere;

};