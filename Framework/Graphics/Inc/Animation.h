#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace NardaEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransfor(float time) const;
		float GetDuration() const;
	private:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		Scalekeys mScaleKeys;
		float mDuration = 0.0f;

	};
}