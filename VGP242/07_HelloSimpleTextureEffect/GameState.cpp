#include "GameState.h"
#include "math.h"
using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	//Initialize gpu communication
	mSimpleTextureEffect.Initialize();

	// initialize rende object 
	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mObject0.mesh.Initialize(sphere);
	mObject0.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");

	//MeshPX sword = MeshBuilder::CreateOBJPX(L"../../Assets/Models/Sword/SlothSword.obj");
	MeshPX sword = MeshBuilder::CreateOBJPX(L"../../Assets/Models/Sword/SlothSword.obj", 0.01f);
	mObject1.mesh.Initialize(sword);

	mObject1.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Models/Sword/SlothSword.png", false);
	mObject1.matWorld = Math::Matrix4::Translation({ 0.0f, 1.0f, 0.0f });

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}
void GameState::Terminate() 
{
	mRenderTarget.Terminate();
	TextureManager::Get()->RealeaseTexture(mObject0.textureId);
	TextureManager::Get()->RealeaseTexture(mObject1.textureId);
	mObject0.mesh.Terminate();
	mObject1.mesh.Terminate();
	mSimpleTextureEffect.Terminate();
}
void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);
}
void GameState::Render() 
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

// render to the render target
	mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
	mRenderTarget.BegingRender();
		mSimpleTextureEffect.Begin();
			mSimpleTextureEffect.Render(mObject0);
			mSimpleTextureEffect.Render(mObject1);
		mSimpleTextureEffect.End();
	mRenderTarget.EndRender();

	//render to the scene
	mSimpleTextureEffect.SetCamera(mCamera);
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mObject0);
	mSimpleTextureEffect.Render(mObject1);
	mSimpleTextureEffect.End();
	
	
}


bool gCheValue = false;
float gFloatVal = 0.0f;
Math::Vector3 gV0 = Math::Vector3::Zero;
Math::Vector3 gV1 = Math::Vector3::One;
Math::Vector3 gV2 = Math::Vector3::XAxis;
Color gColor = Colors::White;

////Combobox
enum class Shape
{
	None,
	AABB,
	AABBFilled,
	Sphere,
	GroundPlane,
	GroundCircle,
	Transform,
};
const char* gShapeNames[] =
{
	"None",
	"AABB",
	"AABBFilled",
	"Sphere",
	"GroundPlane",
	"GroundCircle",
	"Transform",
};

inline Math::Vector3 GetTranslation(const Math::Matrix4& m)
{
	return { m._41, m._42, m._43 };
}

Shape gCurrentShape = Shape::None;
void GameState::DebugUI() 
{
	
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Hello Yall");
	ImGui::Checkbox("IsChecked", &gCheValue);
	ImGui::DragFloat("FloatVal", &gFloatVal);
	ImGui::DragFloat3("V0", &gV0.x, 0.1f);
	ImGui::DragFloat3("V1", &gV1.x, 0.1f);
	ImGui::DragFloat3("V2", &gV2.x, 0.1f);
	ImGui::ColorEdit4("Color", &gColor.r);

	int currentShape = (int)gCurrentShape;
	if (ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
	{
		gCurrentShape = (Shape)currentShape;
	}


	switch (gCurrentShape)
	{
	case Shape::None: break;
	case Shape::AABB:
	{
		ImGui::DragFloat3("Min", &gV0.x, 0.1f);
		ImGui::DragFloat3("Max", &gV1.x, 0.1f);
		SimpleDraw::AddAABB(gV0, gV1, gColor);
		break;
	}
	case Shape::AABBFilled:
	{
		ImGui::DragFloat3("Min", &gV0.x, 0.1f);
		ImGui::DragFloat3("Max", &gV1.x, 0.1f);
		SimpleDraw::AddFilledAABB(gV0, gV1, gColor);
		break;
	}
	case Shape::Sphere:
	{
		ImGui::DragFloat3("Center", &gV0.x, 0.1f);
		SimpleDraw::AddSphere(60, 60, gFloatVal, gColor, gV0);
		break;
	}
	case Shape::GroundPlane:
	{
		/*ImGui::DragFloat("Min");
		ImGui::DragFloat("Max");*/
		SimpleDraw::AddGroundPlane(gFloatVal, gColor);
		break;
	}
	case Shape::GroundCircle:
	{
		ImGui::DragFloat3("Center", &gV0.x, 0.1f);
		SimpleDraw::AddGroundCircle(60, gFloatVal, gColor, gV0);
		break;
	}
	case Shape::Transform:
	{
		SimpleDraw::AddTransform(Math::Matrix4::Identity);
		break;
	}
	}

	//Render Target Camera
	ImGui::Separator();
	ImGui::Text("Render Target");

	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 }, //white color to render target
		{ 1, 1, 1, 1 }); //black border color

	ImGui::End();
	//SimpleDraw::AddGroundPlane(20.0f, Colors::White);//grid
	//SimpleDraw::Render(mCamera);
	float rotation = 0.0f; // rotation of orbit or planet
	Math::Vector3 position = Math::Vector3::Zero;
	Math::Matrix4 matRot = Math::Matrix4::RotationY(rotation);
	Math::Matrix4 matTrans = Math::Matrix4::Translation(position);
	// rotation += dayTimeScale * deltaTime;
	// orbitRotation += yearTimeScale * deltaTime;

	// planet translation = matSunOrbitRotation * matDistanceFromSun;
	// matWorld = planet rotation * planet translation;
	Math::Matrix4 matWorld = matRot * matTrans;
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


