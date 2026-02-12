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
		static MeshPX CreateCubePX(float size);
		static Mesh CreateCube(float size);

		//Make a pyramid 
		static MeshPC CreatePyramidPC(float size);

		//Make a rectangle
		static MeshPC CreateRectanglePC(float width, float height, float depth);

		// Plane 
		static MeshPC CreatePlanePC(int numRows, int numColums, float spacing, bool horizontal = true);
		static MeshPX CreatePlanePX(int numRows, int numColums, float spacing, bool horizontal = true);
		static MeshPX CreatePlaneVerticalPX(int numRows, int numColums, float spacing, bool horizontal = false);
		static Mesh CreatePlane(int numRows, int numColums, float spacing, bool horizontal = true);


		//Cylinder
		static MeshPC CreateCylinderPC(int slices, int rings);

		//Sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);
		static MeshPX CreateSpherePX(int slices, int rings, float radius);
		static Mesh CreateSphere(int slices, int rings, float radius);


		//Sky
		static MeshPX CreateSkySpherePX(int slices, int rings, float radius);

		// load obj
		static MeshPX CreateOBJPX(const std::filesystem::path& filePath, float scale);
		
		// Screen Quad 
		static MeshPX CreateScreenQuadPX();

		
	};
}
