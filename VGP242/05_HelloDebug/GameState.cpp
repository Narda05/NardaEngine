#include "GameState.h"

using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;



void GameState::Initialize() 
{

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
Math::Vector3 gVectorVal = Math::Vector3::Zero;
Color gColor = Colors::White;
void GameState::DebugUI() 
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Hello Yall");
	ImGui::Checkbox("IsChecked", &gCheValue);
	ImGui::DragFloat("Float", &gFloatVal);
	ImGui::DragFloat3("Vector", &gVectorVal.x);
	ImGui::ColorEdit3("Color", &gColor.r);
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