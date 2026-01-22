#pragma once

#include <NardaEngine/Inc/SpEngine.h>

//enum SkeletonToShow
//{
//	Character01,
//	Character02,
//	Character03
//};

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

	NardaEngine::Graphics::RenderGroup mCharacter; 
	NardaEngine::Graphics::RenderGroup mCharacter2; 
	NardaEngine::Graphics::RenderGroup mCharacter3; 
	NardaEngine::Graphics::StandardEffect mStandardEffect;

	bool mDrawSkeleton = false;/*
	SkeletonToShow mSkeletonToShow = SkeletonToShow::Character01;
	SkeletonToShow mPreviousSkeletonToShow = SkeletonToShow::Character01;*/

};