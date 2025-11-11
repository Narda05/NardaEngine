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
	switch (mMode)
	{
	case Mode::None:
	case Mode::Monochrome:
	case Mode::Invert:
		break;
	case Mode::Mirror:
	{
		data.param0 = mMirrorScaleX;
		data.param1 = mMirrorScaleY;
	}
	break;
	case Mode::Blur:
	case Mode::MotionBlur:
	{
		GraphicsSystem* gs = GraphicsSystem::Get();
		const float screenWidth = gs->GetBackBufferWidth();
		const float screenHeight = gs->GetBackBufferHeight();
		data.param0 = mBluerStrength / screenWidth;
		data.param1 = mBluerStrength / screenHeight;
	}
	break; 
	case Mode::Combine2:
	{
		data.param0 = mCombine2Alpha;
	}
	break;
	case Mode::ChromaticAberration:
	{
		data.param0 = mAberrationValue;
		data.param1 = mAberrationValue;
	}
	break;
	case Mode::Wave:
	{
		data.param0 = mWaveLength;
		data.param1 = mNumWaves;
	}
	break;
	default:
		break;
	}

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
		if (mMode == Mode::Mirror)
		{
			ImGui::DragFloat("MirrorScaleX", &mMirrorScaleX, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("MirrorScaleY", &mMirrorScaleY, 0.01f, -1.0f, 1.0f);
		}
		else if(mMode == Mode::Blur || mMode == Mode::MotionBlur)
		{
			ImGui::DragFloat("BlurStrength", &mBluerStrength, 0.1f, 0.0f, 100.0f);
		}
		else if (mMode == Mode::Combine2)
		{
			ImGui::DragFloat("Combine2Alpha", &mCombine2Alpha, 0.01f, 0.0f, 1.0f);
		}
		else if (mMode == Mode::ChromaticAberration)
		{
			ImGui::DragFloat("AberrationValue", &mAberrationValue, 0.0001f, 0.0f, 1.0f);
		}
		else if (mMode == Mode::Wave)
		{
			ImGui::DragFloat("WaveLength", &mWaveLength, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("NumWaves", &mNumWaves, 0.1f, 0.0f, 1000.0f);
		}
	}

}