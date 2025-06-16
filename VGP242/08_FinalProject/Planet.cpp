# include "Planet.h"
using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Math;

void Planet::Initialize(const std::filesystem::path& texturePath, float radius, float orbitDistance, float daySpeed, float yearSpeed)
{
	MeshPX planet = MeshBuilder::CreateSpherePX(60, 60, radius);
	mRenderData.mesh.Initialize(planet);
	mRenderData.textureId = TextureManager::Get()->LoadTexture(texturePath);
	mRenderData.matWorld = Math::Matrix4::Translation({ orbitDistance, 0.0f, 0.0f });

	mOrbitDistance = orbitDistance;
	mDaySpeed = daySpeed;
	mYearSpeed = yearSpeed;
	mCurrentDayRotation = 0.0f;
	mCurrentYearRotation = 0.0f;
}

void Planet::Terminate()
{
	TextureManager::Get()->RealeaseTexture(mRenderData.textureId);
	mRenderData.mesh.Terminate();
}

void Planet::Update(float deltaTime)
{
	mCurrentDayRotation += mDaySpeed * deltaTime;
	mCurrentYearRotation += mYearSpeed * deltaTime;

	Math::Matrix4 matRotation = Math::Matrix4::RotationY(mCurrentDayRotation);
	Math::Matrix4 matTranslation = Math::Matrix4::Translation({ mOrbitDistance, 0.0f, 0.0f });
	Math::Matrix4 matWorld = matRotation * matTranslation;
	mRenderData.matWorld =
		Math::Matrix4::RotationY(mCurrentYearRotation) * 
		Math::Matrix4::Translation({ mOrbitDistance, 0.0f, 0.0f }) *
		Math::Matrix4::RotationY(mCurrentDayRotation);
}

const Graphics::SimpleTextureEffect::RenderData& Planet::GetRenderData() const
{
	return mRenderData;
}
