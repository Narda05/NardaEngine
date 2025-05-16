#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;

namespace 
{
	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] = {
			Colors::Red,
			Colors::AliceBlue,
			Colors::DarkGreen,
			Colors::Orange,
			Colors::Purple,
			Colors::Pink,
			Colors::Yellow,
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}
}

MeshPC MeshBuilder::CreateCubePC(float size, const Color& color)
{
	MeshPC mesh;

	const float hs = size * 0.5f;
	//front 
	mesh.vertices.push_back({ { -hs, -hs, -hs, }, color });
	mesh.vertices.push_back({ { -hs,  hs, -hs, }, color });
	mesh.vertices.push_back({ {  hs,  hs, -hs, }, color });
	mesh.vertices.push_back({ {  hs, -hs, -hs, }, color });
	//back										  
	mesh.vertices.push_back({ { -hs, -hs,  hs, }, color });
	mesh.vertices.push_back({ { -hs,  hs,  hs, }, color });
	mesh.vertices.push_back({ {  hs,  hs,  hs, }, color });
	mesh.vertices.push_back({ {  hs, -hs,  hs, }, color });

	mesh.indices = {
		//front
		0, 1, 2,
		0, 2, 3,
		//back
		7, 5, 4,
		7, 6, 5,
		//right
		3, 2, 6,
		3, 6, 7,
		//left
		4, 5, 1,
		4, 1, 0,
		//top
		1, 5, 6,
		1, 6, 2,
		//bottom
		0, 3, 7,
		0, 7, 4
	};

	return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
	MeshPC mesh;

	int index = rand() % 100;
	const float hs = size * 0.5f;
	//front 
	mesh.vertices.push_back({ { -hs, -hs, -hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ { -hs,  hs, -hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ {  hs,  hs, -hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ {  hs, -hs, -hs, }, GetNextColor(index)});
	//back										  
	mesh.vertices.push_back({ { -hs, -hs,  hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ { -hs,  hs,  hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ {  hs,  hs,  hs, }, GetNextColor(index)});
	mesh.vertices.push_back({ {  hs, -hs,  hs, }, GetNextColor(index)});

	mesh.indices = {
		//front
		0, 1, 2,
		0, 2, 3,
		//back
		7, 5, 4,
		7, 6, 5,
		//right
		3, 2, 6,
		3, 6, 7,
		//left
		4, 5, 1,
		4, 1, 0,
		//top
		1, 5, 6,
		1, 6, 2,
		//bottom
		0, 3, 7,
		0, 7, 4
	};

	return mesh;
}


//Make a piramid
MeshPC NardaEngine::Graphics::MeshBuilder::CreatePyramidPC(float size)
{
	MeshPC mesh;
	int index = rand() % 100;
	const float hs = size * 0.5f;

	//add all the vertices for a pyramid 
	mesh.vertices.push_back({ { -hs, -hs, -hs, }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hs,  hs, -hs, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs,  hs, -hs, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hs, -hs, -hs, }, GetNextColor(index) });
	
	// add all the indices 
	mesh.indices = {
		//front
		0, 1, 2,
		0, 2, 3,
		//back
		4, 5, 6,
		4, 6, 7,
		//right
		3, 2, 6,
		3, 6, 7,
		//left
		4, 5, 1,
		4, 1, 0,
		//top
		1, 5, 6,
		1, 6, 2,
	};

	

	return mesh;
}


//Make a rectangle
MeshPC NardaEngine::Graphics::MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
	MeshPC mesh;
	int index = rand() % 100;
	const float hw = width * 0.5f;
	const float hh = height * 0.5f;
	const float hd = depth * 0.5f;
	//add all the vertices for a rectangle 

	mesh.vertices.push_back({ { -hw, -hh, -hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hw,  hh, -hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw,  hh, -hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw, -hh, -hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hw, -hh,  hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ { -hw,  hh,  hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw,  hh,  hd, }, GetNextColor(index) });
	mesh.vertices.push_back({ {  hw, -hh,  hd, }, GetNextColor(index) });
	
	// add all the indices 
	mesh.indices = {
		//front
		0, 1, 2,
		0, 2, 3,
		//back
		7, 5, 4,
		7, 6, 5,
		//right
		3, 2, 6,
		3, 6, 7,
		//left
		4, 5, 1,
		4, 1, 0,
		//top
		1, 5, 6,
		1, 6, 2,
		//bottom
		0, 3, 7,
		0, 7, 4
	};
	
	return mesh;
}
