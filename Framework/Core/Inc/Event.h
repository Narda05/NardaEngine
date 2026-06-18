#pragma once
#include "Common.h"

namespace NardaEngine::Core
{
	using EventTypeId = std::size_t;

	class Event
	{
	public: 
		Event() = default;
		virtual ~Event() = default;

		virtual EventTypeId GetTypedId() const = 0;
	};
}

#define SET_EVENT_TYPE_ID(id) \
	static NardaEngine::Core::EventTypeId StaticGetTypeId() { return static_cast<NardaEngine::Core::EventTypeId>(id); }\
	NardaEngine::Core::EventTypeId GetTypedId() const override { return StaticGetTypeId(); }

namespace NardaEngine::Core 
{
	class JumpEvent : public Event
	{
	public:
		SET_EVENT_TYPE_ID(static_cast<NardaEngine::Core::EventTypeId>(1));
		JumpEvent(int jumpCount) : mJumpCount(jumpCount) {}
		int GetJumpCount() const { return mJumpCount; }
	private:
		int mJumpCount = 0;
	};
}
