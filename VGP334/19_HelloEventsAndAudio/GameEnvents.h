#pragma once

#include <NardaEngine/Inc/SpEngine.h>

enum class GameEventType
{
	PressSpace = 1, 
	PressEnter
};

class PressSpaceEvent : public NardaEngine::Core::Event
{
public: 
	PressSpaceEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public NardaEngine::Core::Event
{
public:
	PressEnterEvent() {}
	SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};