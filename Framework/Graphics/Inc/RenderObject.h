#pragma once

#include "MeshBuffer.h"
#include "Transform.h"

namespace NardaEngine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;
		MeshBuffer meshbuffer;
	};
}
