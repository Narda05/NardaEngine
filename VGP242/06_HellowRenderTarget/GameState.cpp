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
	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mTransformBuffer.Initialize(sizeof(Math::Matrix4));

	// initialize rende object 
	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mObject0.meshBuffer.Initialize(sphere);
	mObject1.meshBuffer.Initialize(sphere);

	mObject0.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");
	mObject1.textureId = TextureManager::Get()->LoadTexture(L"earth.jpg");
	mObject1.matWorld = Math::Matrix4::Translation({ 0.0f, 1.0f, 0.0f });

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}
void GameState::Terminate() 
{
	mRenderTarget.Terminate();
	TextureManager::Get()->RealeaseTexture(mObject0.textureId);
	TextureManager::Get()->RealeaseTexture(mObject1.textureId);
	mObject0.meshBuffer.Terminate();
	mObject1.meshBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
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
	mRenderTarget.BegingRender();
	RenderObject(mObject0, mRenderTargetCamera);
	RenderObject(mObject1, mRenderTargetCamera);
	mRenderTarget.EndRender();

	//render to the scene
	RenderObject(mObject0, mCamera);
	RenderObject(mObject1, mCamera);
}

void GameState::RenderObject(const Object& object, const Camera& camera)
{
	const Math::Matrix4 matView = camera.GetViewMatrix();
	const Math::Matrix4 matProj = camera.GetProjectionMatrix();
	const Math::Matrix4 matFinal = object.matWorld * matView * matProj;
	const Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(&wvp);

	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mTransformBuffer.BindVS(0);

	TextureManager::Get()->BindPS(object.textureId, 0);
	object.meshBuffer.Render();
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

static int selectedTarget = 0;
const char* targetNames[] = { "Sun", "Earth" };
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

	// Combo to select which object to look at
	static int selectedTarget = 0;
	const char* targetNames[] = { "Sun", "Earth" };
	ImGui::Combo("Camera Target", &selectedTarget, targetNames, IM_ARRAYSIZE(targetNames));
	
    // GetTranslation function is used by explicitly specifying the namespace.  
    Math::Vector3 targetPosition = (selectedTarget == 0)  
       ? NardaEngine::Math::GetTranslation(mObject0.matWorld)  
       : NardaEngine::Math::GetTranslation(mObject1.matWorld);  
    mRenderTargetCamera.SetLookAt(targetPosition);
	mRenderTargetCamera.SetLookAt(targetPosition);

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


