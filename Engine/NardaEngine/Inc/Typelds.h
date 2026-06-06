#pragma once

namespace NardaEngine
{
	enum class ComponentId
	{
		Invalid,                   // default value, not valid Id
		Transform,                 // Transform Component for location data
		Camera,                    // Camera Component for view and projection data
		FPSCamera,                 // move the camera component with FPS controls
		TPCamera,                  // camera that hovers over a controlled object
		Mesh,                      // add a mesh shape on the game object to render
		Model,                     // adds a model to the game object to render
		Animator,                  // adds an animation controller to the game object
		RigidBody,                 // adds a physics object to the game object
		PlayerController,          // input used to move a character/mesh/model
		SoundEffect,               // adds a sound effect to an object
		SoundBank,                 // adds a group of sounds to an object
		UIText,                    // adds a UI text Component
		UISprite,                 // adds a UI BSprite Component
		UIButton,                   // adds a UI Button Component
		Count                      // las value, can be used to chain custom components
	};

	enum class ServiceId
	{
		Invalid,                     // default value, not valid id
		Camera,                      // manages the camera in the world
		Render,                      // renders renderobject in the world
		Physics,                     // registers and monitors physics objects
		UIRender,                     // render UI Components
		Count                        // last value, can be used to chain custom services
	};
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeId() const override { return StaticGetTypeId(); }