#include "Precompiled.h"
#include "Animator.h"
#include "Animation.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;


void Animator::Initialize(ModelId id)
{
	// store model id 
	mModelId = id; 
	mIsLooping = false;
	mAnimationTick = 0.0f;
	// -1 menas not animating 
	mClipIndex = -1;

}
// reset animation, play new one 
void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;

}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.tickPerSecond * deltaTime;
	if (mIsLooping)
	{
		//loop the timer 
		while (mAnimationTick > animClip.tickDuration)
		{
			mAnimationTick -= animClip.tickDuration;
		}
	}
	else
	{
		// if not looping, clamp the end of duration
		mAnimationTick = Math::Min(mAnimationTick, animClip.tickDuration);
	}

}

bool Animator::IsFinished() const
{
	if (mClipIndex < 0 || mIsLooping)
	{
		return false;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

bool Animator::GetToParentTransform(const Bone* bone, Math::Matrix4& transform) const
{
	if(mClipIndex < 0)
	{
		return false;
	}
	
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimations[bone->index].get();
	if(animation == nullptr)
	{
		return false;
	}
	Transform animTransform = animation->GetTransfor(mAnimationTick);
	transform = animTransform.GetMatrix4();
	return true;
}
