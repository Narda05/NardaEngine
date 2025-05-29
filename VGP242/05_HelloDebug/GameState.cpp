#include "GameState.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;



void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

}
void GameState::Terminate() 
{

}
void GameState::Update(float deltaTime) 
{
	UpdateCamera(deltaTime);
}
void GameState::Render() 
{

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

bool gShapeVisible[] = { false, false, false, false, false, false, false };
Math::Vector3 gTransformPosition = Math::Vector3::Zero;
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

	if (gCurrentShape != Shape::None)
	{
		ImGui::Checkbox("Show Shape", &gShapeVisible[currentShape]);
	}

	if (gCurrentShape == Shape::Transform)
	{
		ImGui::DragFloat3("Transform Position", &gTransformPosition.x, 0.1f);
	}

	if (gCurrentShape != Shape::None && gShapeVisible[currentShape])
	{ 
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
			/*ImGui::DragFloat("Min");
			ImGui::DragFloat("Max");*/
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
			/*ImGui::DragFloat("Min");
			ImGui::DragFloat("Max");*/
			SimpleDraw::AddGroundCircle(60, gFloatVal, gColor, gV0);
			break;
		}
		case Shape::Transform: 
		{
			Math::Matrix4 transform = Math::Matrix4::Translation(gTransformPosition);
			SimpleDraw::AddTransform(Math::Matrix4::Identity);
			break;
		}
		default:
			break;
		}
	}
	ImGui::End();
	//SimpleDraw::AddFace( gV0, gV1, gV2, Colors::AliceBlue);
	////SimpleDraw::AddGroundPlane(10.0f, Colors::Red);//grid
	//SimpleDraw::AddTransform(Math::Matrix4::Identity);// the 3 linea blue, red and yellow
	SimpleDraw::Render(mCamera);

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