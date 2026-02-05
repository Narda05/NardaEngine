#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Animator.h"

namespace NardaEngine::Graphics
{
	struct Skeleton;

	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;	// location
		MeshBuffer meshBuffer;	// shape
		Material material;	//light dta
		TextureId diffuseMapId;	//diffuse texture for an object 
		TextureId specMadId;	//specular texture for an object
		TextureId normalMapId; // normal map texture for an object
		TextureId bumpMapId; // bump map texture for an object
	};

	class RenderGroup
	{
	public: 
		void Initialize(const std::filesystem::path& modelFilePath);
		void Terminate(); 

		ModelId modelId; 
		Transform transform;
		std::vector<RenderObject> renderObjects;

		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};
}
