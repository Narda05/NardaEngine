#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Color.h"
#include "SimpleDraw.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

//empty namespace for global functions isolated to the cpp file
namespace
{
	void ComputeBoneTransformsReursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone != nullptr)
		{
			// set the bone transform to the array of matices
			// if no animattor or the bone does not have any animation , use the regular toparenttransform, 
			if (animator == nullptr || !animator->GetToParentTransform(bone, boneTransforms[bone->index]))
			{
				boneTransforms[bone->index] = bone->toParentTransform;
			}
			// if there is a parent, apply the parent's transform as well 
			if( bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];

			}
			// go throught the children and apply their transforms
			for (const Bone* child : bone->children)
			{
				ComputeBoneTransformsReursive(child, boneTransforms, animator);
			}
		}
		
	}
}

void AnimationUtil:: ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator )
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		// resize to sync the number of bones with the matrices
		boneTransforms.resize(model->skeleton->bones.size());
		ComputeBoneTransformsReursive(model->skeleton->root, boneTransforms, animator);

	}
	
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		for (const auto& bone : model->skeleton->bones)
		{
		
			if (bone->parent != nullptr)
			{
				const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 parentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				SimpleDraw::AddLine(bonePos, parentPos, Colors::HotPink);
				SimpleDraw::AddSphere(10, 10, 0.03f, Colors::Blue, bonePos);

			}
		}
	}
}


void AnimationUtil::ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms) {
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}