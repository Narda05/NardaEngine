#pragma once
#include <NardaEngine/Inc/SpEngine.h>

class JumpEvent : public NardaEngine::Core::Event
{
public:
    SET_EVENT_TYPE_ID(static_cast<NardaEngine::Core::EventTypeId>(1));

    JumpEvent(int jumpCount) : mJumpCount(jumpCount) {}

    int GetJumpCount() const { return mJumpCount; }

private:
    int mJumpCount = 0;
};

