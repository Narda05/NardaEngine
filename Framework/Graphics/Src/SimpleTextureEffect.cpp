#include "Precompiled.h"
#include "SimpleTextureEffect.h"

#include "Camera.h"
#include "VertexTypes.h"


using namespace NardaEngine;
using namespace NardaEngine::Graphics;

void SimpleTextureEffect::Initialize()
{
	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mTrandformBuffer.Initialize(sizeof(Math::Matrix4));

}

void SimpleTextureEffect::Terminate()
{
	mTrandformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void SimpleTextureEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mTrandformBuffer.BindVS(0);

}

void SimpleTextureEffect::End()
{
	Texture::UnbindPS(0);
}

void SimpleTextureEffect::Render(const SimpleTextureEffect::RenderData& renderData)
{
	ASSERT(mCamera != nullptr, "SimpleTextureEffec: must have a camera ");
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = renderData.matWorld * matView * matProj;
	const Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTrandformBuffer.Update(&wvp);

	TextureManager::Get()->BindPS(renderData.textureId, 0);
	renderData.mesh.Render();
}

void SimpleTextureEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
