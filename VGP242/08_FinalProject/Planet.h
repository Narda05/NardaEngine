#pragma once
#include <NardaEngine/Inc/SpEngine.h>
 
class Planet
{
public:
	void Initialize(const std::filesystem::path& texturePath, float radius, float orbitDistance, float daySpeed, float yearSpeed);
	void Terminate();
	void Update(float deltaTime);
	const NardaEngine::Graphics::SimpleTextureEffect::RenderData& GetRenderData() const;
	
	float mOrbitDistance = 0.0f;
	float mCurrentYearRotation = 0.0f;
	float mCurrentDayRotation = 0.0f;
	NardaEngine::Graphics::SimpleTextureEffect::RenderData mRenderData;
	float mDaySpeed = 0.0f;
	float mYearSpeed = 0.0f;
private:
};