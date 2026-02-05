#pragma once

#include "Animation.h"

namespace NardaEngine::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float tickDuration = 0.0f;
		float tickPerSecond = 0.0f;
		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}