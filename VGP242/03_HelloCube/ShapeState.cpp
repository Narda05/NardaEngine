#include "ShapeState.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Math;
using namespace NardaEngine::Input;


void ShapeState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 0.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mTransformBuffer.Initialize(sizeof(Math::Matrix4));

	// creates a shape out of the vertices
	CreateShape();
	mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size(), mIndices.data(), mIndices.size());
	
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransformColor.fx";
	mVertexShader.Initialize<VertexPC>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);
}

void ShapeState::Terminate()
{
	
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
	Input::InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{

		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void ShapeState::Render()
{
	// prepare GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	//sync tranform buffer
	mTransformBuffer.BindVS(0);

	// update the buffer data 
	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 matFinal = matWorld * matView * matProj; // wvp
	Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(&wvp);


	// draw
	mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
	const float hs = 0.5f;
	//front 
	mVertices.push_back({ { -hs, -hs, -hs, }, Colors::Red });
	mVertices.push_back({ { -hs,  hs, -hs, }, Colors::Blue });
	mVertices.push_back({ {  hs,  hs, -hs, }, Colors::Green });
	mVertices.push_back({ {  hs, -hs, -hs, }, Colors::LightSkyBlue });
	//back
	mVertices.push_back({ { -hs, -hs,  hs, }, Colors::Red });
	mVertices.push_back({ { -hs,  hs,  hs, }, Colors::Blue });
	mVertices.push_back({ {  hs,  hs,  hs, }, Colors::Green });
	mVertices.push_back({ {  hs, -hs,  hs, }, Colors::LightSkyBlue });

	mIndices = {
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



}
