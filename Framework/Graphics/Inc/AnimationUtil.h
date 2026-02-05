#pragma once

#include "ModelManager.h"
#include "Animator.h"

namespace NardaEngine::Graphics::AnimationUtil
{
	// defining a vector of bone matrices to use for skeleton calculation 
	using BoneTransforms = std::vector<Math::Matrix4>;

	//compute the matices for all the bone in the hierarchy 
	void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator= nullptr);

	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
	// to be called to apply bone offsets for skinning data 
	void ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms);
}