#pragma once

namespace NardaEngine
{
	enum class ComponentId
	{
		Invalid,                   // default value, not valid Id
		Transform,                 // Transform Component for location data
		Camera,                    // Camera Component for view and projection data
		FPSCamera,                 // move the camera component with FPS controls
		Count                  // las value, can be used to chain custom components
	};

	enum class ServiceId
	{
		Invalid,                     // default value, not valid id
		Camera,                      // manages the camera in the world
		Count                        // last value, can be used to chain custom services
	};
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeId() const override { return StaticGetTypeId(); }