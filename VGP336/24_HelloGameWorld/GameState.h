#pragma once

#include <NardaEngine/Inc/SpEngine.h>
#include "SpatialAudioManager.h"

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
	NardaEngine::Audio::SpatialAudioManager mSpatialAudio;

};