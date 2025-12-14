#include "GameState.h"
#include "math.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//Custom the light configuration
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	//Spheres
	Mesh sphereMesh1 = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mSphere01.meshBuffer.Initialize(sphereMesh1);

	Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

	MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
	
	std::filesystem::path shaderFile = "../../Assets/Shaders/ForceField.fx";
	mForceField.Initialize(shaderFile);
	mForceField.SetCamera(mCamera);
	mForceField.SetDirectionalLight(mDirectionalLight);

	shaderFile = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);



}
void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mForceField.Terminate();
	mSphere01.Terminate();
	mGround.Terminate();
}
void GameState::Update(float deltaTime) 
{
	mTime += deltaTime;

	UpdateCamera(deltaTime);

	// Keep emissive fixed to rim color (inner)
	mSphere01.material.emissive.r = mRimColor[0];
	mSphere01.material.emissive.g = mRimColor[1];
	mSphere01.material.emissive.b = mRimColor[2];
	mSphere01.material.emissive.a = mRimColor[3];

	// Center color
	mSphere01.material.ambient.r = mCenterColor[0];
	mSphere01.material.ambient.g = mCenterColor[1];
	mSphere01.material.ambient.b = mCenterColor[2];
	mSphere01.material.ambient.a = mCenterColor[3];

	// Outer rim color stored in specular (shader reads materialSpecular)
	mSphere01.material.specular.r = mOuterRimColor[0];
	mSphere01.material.specular.g = mOuterRimColor[1];
	mSphere01.material.specular.b = mOuterRimColor[2];
	mSphere01.material.specular.a = mOuterRimColor[3];

	// Pass noise params via material.diffuse.xyz
	mSphere01.material.diffuse.r = mNoiseScale;
	mSphere01.material.diffuse.g = mNoiseAmp;
	mSphere01.material.diffuse.b = mRimWidth;
}
void GameState::Render() 
{
	mStandardEffect.Begin();
	mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mForceField.Begin(mTime);
		mForceField.Render(mSphere01);
	mForceField.End();

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("ForceField Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Inner Rim Color (Emissive)", mRimColor);
		ImGui::ColorEdit4("Center Color (Ambient)", mCenterColor);
		//ImGui::ColorEdit4("Outer Rim Color (Specular)", mOuterRimColor);

		ImGui::Text("Noise parameters");
		ImGui::DragFloat("Noise Scale", &mNoiseScale, 0.01f, 0.01f, 10.0f);
		ImGui::DragFloat("Noise Amplitude", &mNoiseAmp, 0.01f, 0.0f, 5.0f);
		ImGui::DragFloat("Rim Width", &mRimWidth, 0.01f, 0.0f, 1.0f);

		ImGui::DragFloat("Shininess#Material", &mSphere01.material.shininess, 0.01f, 0.0f, 1000.0f);
	}
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime) 
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
}


