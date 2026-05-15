#pragma once

#include "CustomTypeIds.h"
#include <NardaEngine/Inc/SpEngine.h>

class CustomDebugDrawComponent;

class CustomDebugDrawService : public NardaEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

    void Render() override;

    void Register(const CustomDebugDrawComponent* debugDrawComponent);
    void Unregister(const CustomDebugDrawComponent* debugDrawComponent);

private:
    using CustomDebugDrawComponents = std::vector<const CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};