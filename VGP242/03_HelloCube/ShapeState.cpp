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
	mMeshBuffer.Initialize(mMesh);
	
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

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("rectangleShapeState");
	}
	else if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("cubeShapeState");
	}
	else if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("pyramidShapeState");
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

void CubeShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreateCubePC(1.0f);
}
void PyramidShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreatePyramidPC(1.0f);
}
void RectangleShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreateRectanglePC(1.0f, 2.0f, 4.0f);
}
void ShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreateCubePC(1.0f);
	mMesh = MeshBuilder::CreatePyramidPC(1.0f);
	mMesh = MeshBuilder::CreateRectanglePC(1.0f, 2.0f, 4.0f);
}
