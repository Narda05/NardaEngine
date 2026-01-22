#pragma once

#include "ModelManager.h"

namespace NardaEngine::Graphics::AnimationUtil
{
	// defining a vector of bone matrices to use for skeleton calculation 
	using BoneTransforms = std::vector<Math::Matrix4>;

	void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms);

	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
	// to be called to apply bone offsets for skinning data 
	void ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms);
}