#pragma once

#include <NardaEngine/Inc/SpEngine.h>

class ShapeState : public NardaEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

	//Create a shape: 1.create the definition of the vertex
private: 
	struct Vertex
	{
		NardaEngine::Math::Vector3 position;
	};
	using Vertices = std::vector<Vertex>;
	Vertices mVertices;

	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;
};