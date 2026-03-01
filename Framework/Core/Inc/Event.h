#pragma once


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