#include "Precompiled.h"
#include "BlendState.h"

#include "GraphicsSystem.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

namespace
{
	D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case BlendState::Mode::Additive:           return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaBlend:         return D3D11_BLEND_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied: return D3D11_BLEND_ONE;
		case BlendState::Mode::Opaque:             return D3D11_BLEND_ONE;
		default:
			ASSERT(false, "BlendState: blend mode not found");
			break;
		}
		return D3D11_BLEND_ONE; // Default fallback
	}
	D3D11_BLEND GetDestBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case BlendState::Mode::Additive:           return D3D11_BLEND_ONE;
		case BlendState::Mode::AlphaBlend:         return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::Mode::AlphaPremultiplied: return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendState::Mode::Opaque:             return D3D11_BLEND_ZERO;
		default:
			ASSERT(false, "BlendState: blend mode not found");
			break;
		}
		return D3D11_BLEND_ZERO;
	}
}

void BlendState::ClearState()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(nullptr, nullptr, UINT_MAX); // Clear blend state
	context->OMSetDepthStencilState(nullptr, 0);
}

BlendState::~BlendState()
{
	ASSERT(mBlendState == nullptr, "BlendState: must call terminate");
}

void BlendState::Initialize(Mode mode)
{
	D3D11_BLEND srcBlend = GetSrcBlend(mode);
	D3D11_BLEND destBlend = GetDestBlend(mode);

	D3D11_BLEND_DESC desc{};
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE || destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = srcBlend;
	desc.RenderTarget[0].DestBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = destBlend;
	desc.RenderTarget[0].SrcBlendAlpha = destBlend;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateBlendState(&desc, &mBlendState);
	ASSERT(SUCCEEDED(hr), "BlendState: failed to create blend state");


	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
	hr = device->CreateDepthStencilState(&dsDesc, &mDepthStencilState);
	ASSERT(SUCCEEDED(hr), "BlendState: failed to create depth stencil state");

}

void BlendState::Terminate()
{
	SafeRelease(mBlendState);
	SafeRelease(mDepthStencilState);
}

void BlendState::Set()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
	context->OMSetDepthStencilState(mDepthStencilState, 0);
}