#include "GameState.h"
#include "math.h"
#include "Planet.h"
using namespace NardaEngine; 
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;
//
//inline Math::Vector3 GetTranslation(const Math::Matrix4& m)
//{
//	return { m._41, m._42, m._43 };
//}

float gOriginalYearSpeeds[10] = { 0.08f, 0.06f, 0.04f, 0.2f, 0.03f, 0.015f, 0.010f, 0.007f, 0.005f, 0.003f };
float gOriginalDaySpeeds[10] = { 0.1f, 0.03f, 0.1f, 0.2f, 0.08f, 0.2f, 0.15f, 0.12f, 0.10f, 0.05f };

bool gShowOrbits = true;
float gOrbitSpeed = 1.0f;
float gRotationSpeed = 1.0f;
int gSelectedPlanet = 0;
const char* gPlanetNames[] = { "Sun", "Mercury", "Venus", "Earth", "Moon", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto" };

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	//Initialize gpu communication
	mSimpleTextureEffect.Initialize();

	//sky sphere 
	CreateShape(); 
	
	MeshPX skySphere = MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
	mSkySphere.mesh.Initialize(skySphere);
	mSkySphere.textureId = TextureManager::Get()->LoadTexture("space.jpg");

	// initialize rende object 
	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mObject0.mesh.Initialize(sphere);
	mObject0.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");

	// Initialize rende object 9 planets
	mPlanets.resize(10);
	// Mercury
	mPlanets[0].Initialize(L"planets/mercury.jpg", 0.25f, 2.0f, 0.1f, 0.08f);
	gOriginalDaySpeeds[0] = 0.1f;
	gOriginalYearSpeeds[0] = 0.08f;

	// Venus
	mPlanets[1].Initialize(L"planets/venus.jpg", 0.35f, 3.0f, 0.03f, 0.06f);
	gOriginalDaySpeeds[1] = 0.03f;
	gOriginalYearSpeeds[1] = 0.06f;

	// Earth
	mPlanets[2].Initialize(L"earth.jpg", 0.40f, 4.0f, 0.1f, 0.04f);
	gOriginalDaySpeeds[2] = 0.1f;
	gOriginalYearSpeeds[2] = 0.04f;

	// Moon
	mPlanets[3].Initialize(L"planets/moon.jpg", 0.12f, 0.7f, 0.2f, 0.2f);
	gOriginalDaySpeeds[3] = 0.2f;
	gOriginalYearSpeeds[3] = 0.2f;

	// Mars
	mPlanets[4].Initialize(L"planets/mars.jpg", 0.30f, 5.5f, 0.08f, 0.03f);
	gOriginalDaySpeeds[4] = 0.08f;
	gOriginalYearSpeeds[4] = 0.03f;

	// Jupiter
	mPlanets[5].Initialize(L"planets/jupiter.jpg", 0.80f, 8.0f, 0.2f, 0.015f);
	gOriginalDaySpeeds[5] = 0.2f;
	gOriginalYearSpeeds[5] = 0.015f;

	// Saturn
	mPlanets[6].Initialize(L"planets/saturn.jpg", 0.70f, 9.0f, 0.15f, 0.010f);
	gOriginalDaySpeeds[6] = 0.15f;
	gOriginalYearSpeeds[6] = 0.010f;

	// Uranus
	mPlanets[7].Initialize(L"planets/uranus.jpg", 0.50f, 10.0f, 0.12f, 0.007f);
	gOriginalDaySpeeds[7] = 0.12f;
	gOriginalYearSpeeds[7] = 0.007f;

	// Neptune
	mPlanets[8].Initialize(L"planets/neptune.jpg", 0.50f, 11.0f, 0.10f, 0.005f);
	gOriginalDaySpeeds[8] = 0.10f;
	gOriginalYearSpeeds[8] = 0.005f;

	// Pluto
	mPlanets[9].Initialize(L"planets/pluto.jpg", 0.15f, 12.0f, 0.05f, 0.003f);
	gOriginalDaySpeeds[9] = 0.05f;
	gOriginalYearSpeeds[9] = 0.003f;
	
	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}
void GameState::Terminate() 
{
	for (Planet& planet : mPlanets)
	{
		planet.Terminate();
	}
	mRenderTarget.Terminate();
	TextureManager::Get()->RealeaseTexture(mObject0.textureId);
	TextureManager::Get()->RealeaseTexture(mObject1.textureId);
	TextureManager::Get()->RealeaseTexture(mSkySphere.textureId);
	
	mObject0.mesh.Terminate();
	mObject1.mesh.Terminate();
	
	mSimpleTextureEffect.Terminate();

}
void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	for (Planet& planet : mPlanets)
	{
		planet.Update(deltaTime);
	}

	NardaEngine::Math::Vector3 focusPos = { 0.0f, 0.0f, 0.0f };
	float planetRadius = 1.0f; // Default radius

	if (gSelectedPlanet == 0) // Sun
	{
		// Sun position (origen)
		focusPos = { mObject0.matWorld._41, mObject0.matWorld._42, mObject0.matWorld._43 };
		planetRadius = 1.0f; //Sun radio 
	}
	else if (gSelectedPlanet == 4) // Moon
	{
		const auto& earthMat = mPlanets[2].GetRenderData().matWorld;
		NardaEngine::Math::Vector3 earthPos = { earthMat._41, earthMat._42, earthMat._43 };
		float moonAngle = mPlanets[3].mCurrentYearRotation;
		float moonDist = mPlanets[3].mOrbitDistance;
		NardaEngine::Math::Vector3 moonOffset = { cosf(moonAngle) * moonDist, 0.0f, sinf(moonAngle) * moonDist };
		focusPos = earthPos + moonOffset;
		planetRadius = 0.1f;
	}
	else
	{
		// Planets: Mercury=1, Venus=2, Earth=3, Mars=5, etc.
		const auto& mat = mPlanets[gSelectedPlanet - 1].GetRenderData().matWorld;
		focusPos = { mat._41, mat._42, mat._43 };
		switch (gSelectedPlanet)
		{
		case 1: planetRadius = 0.3f; break; // Mercury
		case 2: planetRadius = 0.4f; break; // Venus
		case 3: planetRadius = 0.4f; break; // Earth
		case 5: planetRadius = 0.4f; break; // Mars
		case 6: planetRadius = 0.6f; break; // Jupiter
		case 7: planetRadius = 0.5f; break; // Saturn
		case 8: planetRadius = 0.4f; break; // Uranus
		case 9: planetRadius = 0.4f; break; // Neptune
		case 10: planetRadius = 0.4f; break; // Pluto
		}
	}

	// Calculate a suitable distance for the camera
	float minDistance = 2.0f;
	float focusDistance = minDistance;
	if (gSelectedPlanet == 0)
	{
		focusDistance = 3.0f;
	}
	else if (gSelectedPlanet == 4)
	{
		focusDistance = std::max(mPlanets[3].mOrbitDistance * 0.5f + planetRadius * 8.0f, minDistance);
	}
	else
	{
		focusDistance = std::max(planetRadius * 1.2f, minDistance);
	}
	NardaEngine::Math::Vector3 cameraOffset = { 0.0f, 0.5f, -focusDistance };

	mRenderTargetCamera.SetPosition(focusPos + cameraOffset);
	mRenderTargetCamera.SetLookAt(focusPos);
}

void GameState::Render() 
{
	//SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

// render to the render target
	mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
	// the canvas
	mRenderTarget.BegingRender();
		mSimpleTextureEffect.Begin();
			if (gSelectedPlanet == 0) // Sun
			{
				mSimpleTextureEffect.Render(mObject0);
			}
			else if (gSelectedPlanet == 4) // Moon
			{
				const auto& earthMat = mPlanets[2].GetRenderData().matWorld;
				NardaEngine::Math::Vector3 earthPos = { earthMat._41, earthMat._42, earthMat._43 };
				float moonAngle = mPlanets[3].mCurrentYearRotation;
				float moonDist = mPlanets[3].mOrbitDistance;
				NardaEngine::Math::Vector3 moonOffset = { cosf(moonAngle) * moonDist, 0.0f, sinf(moonAngle) * moonDist };

				Math::Matrix4 moonRot = Math::Matrix4::RotationY(mPlanets[3].mCurrentDayRotation);
				Math::Matrix4 moonTrans = Math::Matrix4::Translation(earthPos + moonOffset);

				NardaEngine::Graphics::SimpleTextureEffect::RenderData moonData = mPlanets[3].GetRenderData();
				moonData.matWorld = moonRot * moonTrans;
				mSimpleTextureEffect.Render(moonData);
			}
			else
			{
				mSimpleTextureEffect.Render(mPlanets[gSelectedPlanet - 1].GetRenderData());
			}

		mSimpleTextureEffect.End();
	mRenderTarget.EndRender();

	//render to the scene
	// the main scene that renders the game
	mSimpleTextureEffect.SetCamera(mCamera);
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.Render(mObject0);
	mSimpleTextureEffect.Render(mObject1);
	
	printf("Earth year rot: %f\n", mPlanets[2].mCurrentYearRotation);

	for (size_t i = 0; i < mPlanets.size(); ++i)
	{
		if (i == 3) // Luna
		{
			// Calculate the earth position
			const auto& earthMat = mPlanets[2].GetRenderData().matWorld;
			NardaEngine::Math::Vector3 earthPos = { earthMat._41, earthMat._42, earthMat._43 };
			float moonAngle = mPlanets[3].mCurrentYearRotation;
			float moonDist = mPlanets[3].mOrbitDistance;
			NardaEngine::Math::Vector3 moonOffset = { cosf(moonAngle) * moonDist, 0.0f, sinf(moonAngle) * moonDist };

			// Rotation and translation matrix of the moon
			Math::Matrix4 moonRot = Math::Matrix4::RotationY(mPlanets[3].mCurrentDayRotation);
			Math::Matrix4 moonTrans = Math::Matrix4::Translation(earthPos + moonOffset);

			NardaEngine::Graphics::SimpleTextureEffect::RenderData moonData = mPlanets[3].GetRenderData();
			moonData.matWorld = moonRot * moonTrans;
			mSimpleTextureEffect.Render(moonData);
		}
		else
		{
			mSimpleTextureEffect.Render(mPlanets[i].GetRenderData());
		}
	}

	mSimpleTextureEffect.End();

	//Draw Orbits
	if (gShowOrbits)
	{
		for (size_t i = 0; i < mPlanets.size(); ++i)
		{
			if (i != 3)
				SimpleDraw::AddGroundCircle(100, mPlanets[i].mOrbitDistance, Colors::Gray, Math::Vector3::Zero);
		}
	}
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
//
//inline Math::Vector3 GetTranslation(const Math::Matrix4& m)
//{
//	return { m._41, m._42, m._43 };
//}

Shape gCurrentShape = Shape::None;
void GameState::DebugUI() 
{
	
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	/*ImGui::Text("Hello Yall");
	ImGui::Checkbox("IsChecked", &gCheValue);
	ImGui::DragFloat("FloatVal", &gFloatVal);
	ImGui::DragFloat3("V0", &gV0.x, 0.1f);
	ImGui::DragFloat3("V1", &gV1.x, 0.1f);
	ImGui::DragFloat3("V2", &gV2.x, 0.1f);*/
	ImGui::ColorEdit4("Color", &gColor.r);
	ImGui::Checkbox("Show Orbits", &gShowOrbits);

	static bool useCustomSpeeds = false;
	ImGui::Checkbox("Original velocity", &useCustomSpeeds);

	if (useCustomSpeeds)
	{
		ImGui::SliderFloat("RotationSpeed (Day)", &gOrbitSpeed, 0.001f, 0.2f);
		ImGui::SliderFloat("Orbit Speed (Year)", &gRotationSpeed, 0.001f, 0.2f);
		for (Planet& planet : mPlanets)
		{
			planet.mYearSpeed = gOrbitSpeed;
			planet.mDaySpeed = gRotationSpeed;
		}
	}
	else
	{
		// Restores original speeds
		for (int i = 0; i < mPlanets.size(); ++i)
		{
			mPlanets[i].mYearSpeed = gOriginalYearSpeeds[i];
			mPlanets[i].mDaySpeed = gOriginalDaySpeeds[i];
		}
	}

	/*int currentShape = (int)gCurrentShape;
	if (ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
	{
		gCurrentShape = (Shape)currentShape;
	}*/


	//switch (gCurrentShape)
	//{
	//case Shape::None: break;
	//case Shape::AABB:
	//{
	//	ImGui::DragFloat3("Min", &gV0.x, 0.1f);
	//	ImGui::DragFloat3("Max", &gV1.x, 0.1f);
	//	SimpleDraw::AddAABB(gV0, gV1, gColor);
	//	break;
	//}
	//case Shape::AABBFilled:
	//{
	//	ImGui::DragFloat3("Min", &gV0.x, 0.1f);
	//	ImGui::DragFloat3("Max", &gV1.x, 0.1f);
	//	SimpleDraw::AddFilledAABB(gV0, gV1, gColor);
	//	break;
	//}
	//case Shape::Sphere:
	//{
	//	ImGui::DragFloat3("Center", &gV0.x, 0.1f);
	//	SimpleDraw::AddSphere(60, 60, gFloatVal, gColor, gV0);
	//	break;
	//}
	//case Shape::GroundPlane:
	//{
	//	/*ImGui::DragFloat("Min");
	//	ImGui::DragFloat("Max");*/
	//	SimpleDraw::AddGroundPlane(gFloatVal, gColor);
	//	break;
	//}
	//case Shape::GroundCircle:
	//{
	//	ImGui::DragFloat3("Center", &gV0.x, 0.1f);
	//	SimpleDraw::AddGroundCircle(60, gFloatVal, gColor, gV0);
	//	break;
	//}
	//case Shape::Transform:
	//{
	//	SimpleDraw::AddTransform(Math::Matrix4::Identity);
	//	break;
	//}
	//}

	//Select planet to focus
	ImGui::Combo("Focus Planet", &gSelectedPlanet, gPlanetNames, IM_ARRAYSIZE(gPlanetNames));

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
	
	//float rotation = 0.0f; // rotation of orbit or planet
	//Math::Vector3 position = Math::Vector3::Zero;
	//Math::Matrix4 matRot = Math::Matrix4::RotationY(rotation);
	//Math::Matrix4 matTrans = Math::Matrix4::Translation(position); 
	
	// rotation += dayTimeScale * deltaTime;
	// orbitRotation += yearTimeScale * deltaTime;

	// planet translation = matSunOrbitRotation * matDistanceFromSun;
	// matWorld = planet rotation * planet translation;
	/*Math::Matrix4 matWorld = matRot * matTrans;*/
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

void GameState::CreateShape()
{
	// create a sky sphere
	mMesh = MeshBuilder::CreateSpherePX(60, 60, 100.0f);
}