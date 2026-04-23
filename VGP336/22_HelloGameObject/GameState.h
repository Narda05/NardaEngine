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
	NardaEngine::GameWorld mGameWorld;
	NardaEngine::CameraComponent* mCameraComponent = nullptr;
	NardaEngine::TransformComponent* mPlayer1Transform = nullptr;
	NardaEngine::TransformComponent* mPlayer2Transform = nullptr;
	NardaEngine::TransformComponent* mPlayer3Transform = nullptr;
	NardaEngine::TransformComponent* mPlayer4Transform = nullptr;
};