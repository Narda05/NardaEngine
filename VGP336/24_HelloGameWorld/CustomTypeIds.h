#pragma once

#include <NardaEngine/Inc/Typelds.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<int>(NardaEngine::ComponentId::Count)

};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<int>(NardaEngine::ServiceId::Count)
};

