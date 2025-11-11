#include "Precompiled.h"
#include "PostProcessingEffect.h"	

#include "RenderObject.h"
#include "Texture.h"
#include "VertexTypes.h"
#include "GraphicsSystem.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

namespace
{
	const char* gModeNames[] = 
	{
		"None",
		"Monochrome",
		"Invert",
		"Mirror",
		"Blur",
		"Combine2",
		"MonitorBlur",
		"ChromaticAberration",
		"Wave"
	};
}

void PostProcessingEffect::Initialize(const std::filesystem::path& effectPath)
{
	mVertexShader.Initialize<VertexPX>(effectPath); 
	mPixelShader.Initialize(effectPath);
	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mPostProcessBuffer.Initialize();

}
	

void PostProcessingEffect::Terminate()
{
	mPostProcessBuffer.Terminate();
	mSampler.Terminate(); 
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for(uint32_t i = 0 ; i < mTexture.size(); ++i)
	{
		if (mTexture[i] != nullptr)
		{
			mTexture[i]->BindPS(i);
		}
	}
	PostProcessData data{};
	data.mode = static_cast<int>(mMode);

	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);
}

void PostProcessingEffect::End()
{
	for(uint32_t i = 0 ; i < mTexture.size(); ++i)
	{
		Texture::UnbindPS(i);
	}
}

void PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTexture.size(), "PostProcessingEffec invalid slot index");
	mTexture[slot] = texture;
}

void PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void PostProcessingEffect::DebugUI()
{
	if(ImGui::CollapsingHeader("PostProcessing Effect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		if(ImGui::Combo("Mode", &currentMode, gModeNames, std::size(gModeNames)))
		{
			mMode = static_cast<Mode>(currentMode);
		}
	}
}