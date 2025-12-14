#include "Precompiled.h"
#include "ForceField.h"
#include "Camera.h"

#include "VertexTypes.h"
#include "RenderObject.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

void ForceField::Initialize(const std::filesystem::path& path)
{//buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	//other stuff
	mVertexShader.Initialize<Vertex>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
	mForceFieldBuffer.Initialize();
}

void ForceField::Terminate()
{
	mForceFieldBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mBlendState.Terminate();
}

void ForceField::Begin(float time)
{
	// bind shaders / states

	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindVS(0);
	mSampler.BindPS(0);
	mBlendState.Set();

	// bind common buffers
	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	ForceFieldData forceFieldData{};
	forceFieldData.param0 = time;
	mForceFieldBuffer.Update(forceFieldData);
	mForceFieldBuffer.BindPS(4);
}

void ForceField::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
	mBlendState.ClearState();
}

void ForceField::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matWorld * matView * matProj;


	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matWorld);
	data.viewPosition = mCamera->GetPosition();
	if (mShadowMap != nullptr && mSettingsData.useShadowMap > 0)
	{
		const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
		const Math::Matrix4 matLightProj = mLightCamera->GetProjectionMatrix();
		data.lwvp = Math::Transpose(matWorld * matLightView * matLightProj);
		mShadowMap->BindPS(4);
	}
	mTransformBuffer.Update(data);

	SettingsData settings;
	settings.useDiffuseMap = (renderObject.diffuseMapId > 0 && mSettingsData.useDiffuseMap > 0) ? 1 : 0;
	settings.useSpecMap = (renderObject.specMadId > 0 && mSettingsData.useSpecMap > 0) ? 1 : 0;
	settings.useNormalMap = (renderObject.normalMapId > 0 && mSettingsData.useNormalMap > 0) ? 1 : 0;
	settings.useBumpMap = (renderObject.bumpMapId > 0 && mSettingsData.useBumpMap > 0) ? 1 : 0;
	settings.bumpWeight = mSettingsData.bumpWeight;
	settings.useShadowMap = (mShadowMap != nullptr && mSettingsData.useShadowMap > 0) ? 1 : 0;
	settings.depthBias = mSettingsData.depthBias;
	mSettingsBuffer.Update(settings);

	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMadId, 1);
	tm->BindPS(renderObject.normalMapId, 2);
	tm->BindVS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();
}

void ForceField::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
void ForceField::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}
void ForceField::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}
void ForceField::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
		{
			mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;
		}
		bool useSpecMap = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecularMap", &useSpecMap))
		{
			mSettingsData.useSpecMap = (useSpecMap) ? 1 : 0;
		}
		bool useNormalMap = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
		{
			mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;
		}
		bool useBumpMap = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
		{
			mSettingsData.useBumpMap = (useBumpMap) ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);
		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("UseShadowMap", &useShadowMap))
		{
			mSettingsData.useShadowMap = (useShadowMap) ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
	}
}