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
	
	NardaEngine::Graphics::Camera mCamera;
	NardaEngine::Graphics::DirectionalLight mDirectionalLight;
	NardaEngine::Graphics::StandardEffect mStandardEffect;

	NardaEngine::Graphics::RenderObject mGroundObject;
	NardaEngine::Physics::CollisionShape mGroundShape;
	NardaEngine::Physics::RigidBody mGroundRigidBody;

	struct Segment
	{
		NardaEngine::Graphics::RenderObject segment;
		float range = 0.0f;
	};
	std::vector<Segment>mSegments;

	NardaEngine::Physics::CollisionShape mBodyAnchorShape;
	NardaEngine::Physics::RigidBody mBodyAnchor;
};