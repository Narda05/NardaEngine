#pragma once
#include "CustomTypeIds.h"
#include <NardaEngine/Inc/SpEngine.h>

class CustomDebugDrawComponent : public NardaEngine::Component
{
public:

	SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void DebugUI() override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const NardaEngine::TransformComponent* mTransformComponent = nullptr;
    NardaEngine::Math::Vector3 mPosition = NardaEngine::Math::Vector3::Zero;
    NardaEngine::Graphics::Color mColor = NardaEngine::Graphics::Colors::White;

    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0;
};