#include "ShapeState.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Math;


void ShapeState::Initialize()
{
	// creates a shape out of the vertices
	CreateShape();


	auto device = GraphicsSystem::Get()->GetDevice();

	// need to create a buffer to store the vertices
	// STORE DATA FOR THE OBJETCT
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(mVertices.size()) * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
	//=================================================================================

	//Bind to function in sprecified shader file 
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		shaderFilePath.c_str(), 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0,
		& shaderBlob, 
		& errorBlob);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");
	//=================================================================================

	//STATE WHAT THE VERTERX VARIABLES ARE
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
	//=================================================================================
	
	//BIND TO PIXEL FUCTION IN SPECIFIED SHADER FILE 

	hr = D3DCompileFromFile(
		shaderFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void ShapeState::Terminate()
{
	mVertices.clear();
	SafeRelease(mPixelShader);
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
	SafeRelease(mVertexBuffer);
}

void ShapeState::Update(float deltaTime)
{

	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("TriangleShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("HeartShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("FishShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("StarShapeState");
	}
}

void ShapeState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();
	//bind buffers
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->IASetInputLayout(mInputLayout);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//draw
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->Draw(static_cast<UINT>(mVertices.size()), 0);
}

void ShapeState::CreateShape()
{
	// create a triangle
	mVertices.push_back({ { -0.5f, -0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Green });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, -0.75f, 0.0f }, Colors::Green });
}

void TriangleShapeState::Update(float deltaTime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("TriangleShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("HeartShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("FishShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("StarShapeState");
	}
}

void HeartShapeState::Update(float deltatime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("TriangleShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("HeartShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("FishShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("StarShapeState");
	}
}

void FishShapeState::Update(float deltatime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("TriangleShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("HeartShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("FishShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("StarShapeState");
	}
}

void StarShapeState::Update(float deltatime)
{
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::DOWN))
	{
		NardaEngine::MainApp().ChangeState("TriangleShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::UP))
	{
		NardaEngine::MainApp().ChangeState("HeartShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::LEFT))
	{
		NardaEngine::MainApp().ChangeState("FishShapeState");
	}
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::RIGHT))
	{
		NardaEngine::MainApp().ChangeState("StarShapeState");
	}
}

void TriangleShapeState::CreateShape()
{
	// Example teacheer
	mVertices.push_back({ { -0.75f, -0.75f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { -0.25f, -0.75f, 0.0f }, Colors::Green });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, 0.75f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Green });

	mVertices.push_back({ { 0.25f, -0.75f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.75f, -0.75f, 0.0f }, Colors::Green });

}

void HeartShapeState::CreateShape()
{
	//Heart
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.25f, 0.5f, 0.0f }, Colors::Red });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Red });

	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.25f, 0.5f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Green });
	mVertices.push_back({ { 0.5f, 0.0f, 0.0f }, Colors::Green });
	mVertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::Green });

}
void FishShapeState::CreateShape()
{
	//Fish
	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.8f, 0.6f, 0.0f }, Colors::Red });
	mVertices.push_back({ { -0.1f, 0.0f, 0.0f }, Colors::Red });

	mVertices.push_back({ { -0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { -0.1f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { -0.8f, -0.6f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { -0.1f, -0.5f, 0.0f }, Colors::Green });
	mVertices.push_back({ { -0.1f, 0.5f, 0.0f }, Colors::Green });
	mVertices.push_back({ { 0.9f, 0.0f, 0.0f }, Colors::Green });


}
void StarShapeState::CreateShape()
{
	//Star
	mVertices.push_back({ { -0.6f, -0.6f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.6f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.6f, -0.6f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { -0.6f, 0.3f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, 0.3f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, -0.8f, 0.0f }, Colors::Blue });

	mVertices.push_back({ { 0.0f, 0.3f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.6f, 0.3f, 0.0f }, Colors::Blue });
	mVertices.push_back({ { 0.0f, -0.8f, 0.0f }, Colors::Blue });
	
}


