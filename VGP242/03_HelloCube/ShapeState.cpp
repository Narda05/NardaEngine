#include "ShapeState.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Math;


void ShapeState::Initialize()
{
	// creates a shape out of the vertices
	CreateShape();
	mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());
	
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";
	mVertexShader.Initialize<VertexPC>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);
}

void ShapeState::Terminate()
{
	mVertices.clear();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{

}

void ShapeState::Render()
{
	// prepare GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	// draw
	mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
	// create a triangle
	mVertices.push_back({ { -0.5f, -0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Green });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Green });
}