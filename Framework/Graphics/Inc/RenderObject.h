#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"

namespace NardaEngine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;	// location
		MeshBuffer meshBuffer;	// shape
		Material material;	//light dta
		TextureId diffuseMapId;	//diffurse texture for an object 
		TextureId specMadId;	//specular texture for an object
	};
}
