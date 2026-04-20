#pragma once
#include "Common.h"

#include "App.h"
#include "AppState.h"


// Game Object info 
#include "GameObject.h"
#include "GameWorld.h"
#include "GameObjectHandle.h"

// Components
#include "Typelds.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"

// Services
#include "Service.h"

//for clean and optimitation is creeate a acces
namespace NardaEngine
{
	App& MainApp();
}