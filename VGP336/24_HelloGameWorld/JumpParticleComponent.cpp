#include "JumpParticleComponent.h"
#include "JumpEvent.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Physics;
using namespace NardaEngine::Math;

JumpParticleComponent* JumpParticleComponent::sInstance = nullptr;

void JumpParticleComponent::Initialize()
{
    sInstance = this;
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();


    auto* cameraService = GetOwner().GetWorld().GetService<CameraService>();
    mParticleEffect.Initialize();
	mParticleEffect.SetCamera(cameraService->GetMain());

    //FIrst Jump
    ParticleSystemInfo jumpInfo;
    jumpInfo.textureId = TextureManager::Get()->LoadTexture("Images/white.jpg");
    jumpInfo.maxParticles = 50;
    jumpInfo.particlesPerEmit = { 10, 20 };
    jumpInfo.delay = 0.0f;
    jumpInfo.lifeTime = 0.5f;
    jumpInfo.timeBetweenEmit = { 0.01f, 0.05f };
    jumpInfo.spawnAngle = { -60.0f, 60.0f };
    jumpInfo.spawnSpeed = { 3.0f, 6.0f };
    jumpInfo.particleLifeTime = { 0.2f, 0.5f };
    jumpInfo.spawnDirection = Vector3::YAxis;
    jumpInfo.startScale = { Vector3::One, Vector3::One };
    jumpInfo.endScale = { Vector3::Zero, Vector3::Zero };
    jumpInfo.startColor = { Colors::White, Colors::White };
    jumpInfo.endColor = { Colors::Yellow, Colors::Yellow };
    mJumpParticles.Initialize(jumpInfo);

    //Second Jump
    ParticleSystemInfo doubleJumpInfo;
    doubleJumpInfo.textureId = TextureManager::Get()->LoadTexture("Images/white.jpg");
    doubleJumpInfo.maxParticles = 100;
    doubleJumpInfo.particlesPerEmit = { 20, 40 };
    doubleJumpInfo.delay = 0.0f;
    doubleJumpInfo.lifeTime = 0.5f;
    doubleJumpInfo.timeBetweenEmit = { 0.01f, 0.02f };
    doubleJumpInfo.spawnAngle = { -90.0f, 90.0f };
    doubleJumpInfo.spawnSpeed = { 5.0f, 10.0f };
    doubleJumpInfo.particleLifeTime = { 0.3f, 0.6f };
    doubleJumpInfo.spawnDirection = Vector3::YAxis;
    doubleJumpInfo.startScale = { Vector3::One, Vector3::One * 2.0f };
    doubleJumpInfo.endScale = { Vector3::Zero, Vector3::Zero };
    doubleJumpInfo.startColor = { Colors::Cyan, Colors::White };
    doubleJumpInfo.endColor = { Colors::Blue, Colors::Cyan };
    mDoubleJumpParticles.Initialize(doubleJumpInfo);

    // Listen for jump events
    mJumpListenerId = Core::EventManager::Get()->AddListener(
        Core::JumpEvent::StaticGetTypeId(),
        [this](const Core::Event& e) { OnJump(e); }
    );
}

void JumpParticleComponent::Terminate()
{
    sInstance = nullptr;
    Core::EventManager::Get()->RemoveListener(
        Core::JumpEvent::StaticGetTypeId(), mJumpListenerId);
    mJumpParticles.Terminate();
    mDoubleJumpParticles.Terminate();
    mParticleEffect.Terminate();
    mTransformComponent = nullptr;
}
void JumpParticleComponent::Update(float deltaTime)
{
    mJumpParticles.Update(deltaTime);
    mDoubleJumpParticles.Update(deltaTime);
}
void JumpParticleComponent::Render()
{
    mParticleEffect.Begin();
    mJumpParticles.Render(mParticleEffect);
    mDoubleJumpParticles.Render(mParticleEffect);
    mParticleEffect.End();
}

void JumpParticleComponent::OnJump(const NardaEngine::Core::Event& e) 
{
    const auto& jumpEvent = static_cast<const Core::JumpEvent&>(e);

    if (mTransformComponent != nullptr)
    {
        Math::Vector3 pos = mTransformComponent->position;

        if (jumpEvent.GetJumpCount() == 1)
        {
            // First jump - small burst
            mJumpParticles.SetPosition(pos);
            mJumpParticles.SpawnParticles();
        }
        else
        {
            // Double jump - big cyan burst
            mDoubleJumpParticles.SetPosition(pos);
            mDoubleJumpParticles.SpawnParticles();
        }
    }
}
