#pragma once

#include "ModelManager.h"

namespace NardaEngine::Graphics
{
	struct Bone;
	class Animator
	{
	public: 
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);

		void PlayEvents(float prevTime, float curTime);

		bool IsFinished() const;
		size_t GetAnimationCount() const;
		bool GetToParentTransform(const Bone* bone, Math::Matrix4& transform) const;

	private:
		ModelId mModelId = 0;
		int mClipIndex = -1;
		float mAnimationTick = 0.0f;
		EventKeys mEventKeys;
		bool mIsLooping = false;
	};
}