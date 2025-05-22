#include "ShapeState.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Math;
using namespace NardaEngine::Input;


void ShapeState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mTransformBuffer.Initialize(sizeof(Math::Matrix4));

	// creates a shape out of the vertices
	CreateShape();
	mMeshBuffer.Initialize(mMesh);
	
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFilePath);
	mPixelShader.Initialize(shaderFilePath);

	mTexture.Initialize(L"../../Assets/Textures/earth.jpg");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeState::Terminate()
{
	mSampler.Terminate();
	mTexture.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
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
	/*if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("createspherepx");
	}
	else if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("createskyspherepx");
	}
	else if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("createplanepx");
	}
	else if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("createplaneverticalpx");
	}*/
}

void ShapeState::Render()
{
	// prepare GPU
	mVertexShader.Bind();
	mPixelShader.Bind();

	mSampler.BindPS(0);
	mTexture.BindPS(0);

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

void SphereShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
}
void SkySphereShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreateSkySpherePX(30, 30, 200.0f);
}
void PlaneShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreatePlanePX(30, 30, 1.0f);
}
void PlaneVerticalShapeState::CreateShape()
{
	mMesh = MeshBuilder::CreatePlaneVerticalPX(30, 30, 1.0f);
}



void ShapeState::CreateShape()
{
	//mMesh = MeshBuilder::CreateCubePC(1.0f);
	//mMesh = MeshBuilder::CreatePyramidPC(1.0f);
	//mMesh = MeshBuilder::CreateRectanglePC(1.0f, 2.0f, 4.0f);
	//mMesh = MeshBuilder::CreatePlanePC(10, 10, 1.0f); //horizontal like piso, if you use false it will be vertical like pared. 
	//mMesh = MeshBuilder::CreateCylinderPC(20, 3);
	//mMesh = MeshBuilder::CreateSpherePC(30, 30, 1.0f);
	//mMesh = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
	/*mMesh = MeshBuilder::CreateSkySpherePX(30, 30, 200.0f);*/
	//mMesh = MeshBuilder::CreatePlanePX(30, 30, 1.0f);
	mMesh = MeshBuilder::CreatePlaneVerticalPX(30, 30, 1.0f);
}
