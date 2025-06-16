#pragma once

#include <NardaEngine/Inc/SpEngine.h>
#include "Planet.h"

class GameState : public NardaEngine::AppState
{
public:
void Initialize() override;
void Terminate() override;
void Update(float deltaTime) override;
void Render() override;
void DebugUI() override;

private:

virtual void CreateShape(); // Removed 'override' as it does not override a base class member
void UpdateCamera(float deltaTime);

NardaEngine::Graphics::Camera mCamera;
NardaEngine::Graphics::Camera mRenderTargetCamera;

//GPU communication 
NardaEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

//render object
NardaEngine::Graphics::SimpleTextureEffect::RenderData mObject0;
NardaEngine::Graphics::SimpleTextureEffect::RenderData mObject1;

std::vector<Planet> mPlanets;


// render target
NardaEngine::Graphics::RenderTarget mRenderTarget;

//Sky Sphere
NardaEngine::Graphics::MeshPX mMesh;
NardaEngine::Graphics::SimpleTextureEffect::RenderData mSkySphere;

};
