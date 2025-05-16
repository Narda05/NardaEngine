#pragma once

#include "MeshTypes.h"

namespace NardaEngine::Graphics
{

	class MeshBuilder
	{
	public:
		//Make a cube
		static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

		//Make a pyramid 
		static MeshPC CreatePyramidPC(float size);

		//Make a rectangle
		static MeshPC CreateRectanglePC(float width, float height, float depth);
	};
}