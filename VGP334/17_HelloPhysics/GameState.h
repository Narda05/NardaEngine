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

	//First Render Object
	NardaEngine::Graphics::RenderObject mBallObject;
	NardaEngine::Physics::CollisionShape mBallShape;
	NardaEngine::Physics::RigidBody mBallRigidBody;

	NardaEngine::Graphics::RenderObject mGroundObject;
	NardaEngine::Physics::CollisionShape mGroundShape;
	NardaEngine::Physics::RigidBody mGroundRigidBody;

	struct BoxData
	{
		NardaEngine::Graphics::RenderObject box;
		NardaEngine::Physics::CollisionShape shape;
		NardaEngine::Physics::RigidBody rigidBody;
	};
	using Boxes = std::vector<BoxData>;
	Boxes mBoxes;

	NardaEngine::Graphics::StandardEffect mStandardEffect;

	// cloth info
	NardaEngine::Graphics::RenderObject mCloth;
	NardaEngine::Graphics::Mesh mClothMesh;
	NardaEngine::Physics::SoftBody mClothSoftBody;
};