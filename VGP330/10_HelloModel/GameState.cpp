#include "GameState.h"
#include "math.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;

static int selectedTarget = 0;
const char* targetNames[] = { "Sun", "Earth" };
void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	//Custom the light configuration
	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	//First Render Object
	Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mRenderObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.specMadId = tm->LoadTexture("earth_spec.jpg");
	mRenderObject.normalMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");

	//Second Render Object
	mRenderObject.transform.position = { 0.0f, 0.0f, 0.0f };
	Mesh mesh2 = MeshBuilder::CreateSphere(30, 30, 0.5f);
	mRenderObject2.meshBuffer.Initialize(mesh2);
	mRenderObject2.diffuseMapId = tm->LoadTexture("rockytwo.jpg");        
	mRenderObject2.specMadId = tm->LoadTexture("rockytwo_spec.jpg");
	mRenderObject2.normalMapId = tm->LoadTexture("rockytwo_normal.jpg");
	mRenderObject2.bumpMapId = tm->LoadTexture("rockytwo_bump.jpg");
	mRenderObject2.transform.position = { 2.0f, 0.0f, 0.0f };
	mRenderObject2.transform.scale = { 0.5f, 0.5f, 0.5f };

	//Third render Object
	Mesh planeMesh = MeshBuilder::CreatePlane(10, 10, 1.0f, true);
	mRenderObject3.meshBuffer.Initialize(planeMesh);
	mRenderObject3.diffuseMapId = tm->LoadTexture("rocky.jpg");   
	mRenderObject3.specMadId = tm->LoadTexture("rocky_spec.jpg");
	mRenderObject3.normalMapId = tm->LoadTexture("rocky_normal.jpg");
	mRenderObject3.bumpMapId = tm->LoadTexture("rocky_bump.jpg");
	mRenderObject3.transform.position = { -2.0f, 0.0f, 0.0f };
	mRenderObject3.transform.scale = { 2.0f, 1.0f, 2.0f };



	std::filesystem::path effectPath = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(effectPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate() 
{
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
	mRenderObject2.Terminate();
	mRenderObject3.Terminate();
}
void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);
}
void GameState::Render() 
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
	mStandardEffect.Render(mRenderObject);
	mStandardEffect.Render(mRenderObject2);
	mStandardEffect.Render(mRenderObject3);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Emissive#Material", &mRenderObject.material.emissive.r);
		ImGui::ColorEdit4("Ambient#Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse#Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular#Material", &mRenderObject.material.specular.r);
		ImGui::DragFloat("Shininess#Material", &mRenderObject.material.shininess, 0.1f, 0.0f, 10000.0f);
	}
	if (ImGui::CollapsingHeader("Sphere 1", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Position##Sphere1", &mRenderObject.transform.position.x, 0.1f);
		ImGui::DragFloat3("Scale##Sphere1", &mRenderObject.transform.scale.x, 0.01f, 0.01f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Sphere 2", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Position##Sphere2", &mRenderObject2.transform.position.x, 0.1f);
		ImGui::DragFloat3("Scale##Sphere2", &mRenderObject2.transform.scale.x, 0.01f, 0.01f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Plane", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Plane Position", &mRenderObject3.transform.position.x, 0.1f);
		ImGui::DragFloat3("Plane Scale", &mRenderObject3.transform.scale.x, 0.1f, 0.01f, 100.0f);
	}

	mStandardEffect.DebugUI();
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


