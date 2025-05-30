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
protected:
	virtual void CreateShape();
	
	NardaEngine::Graphics::MeshPC mMesh;
	NardaEngine::Graphics::Camera mCamera;
	NardaEngine::Graphics::ConstantBuffer mTransformBuffer;
	NardaEngine::Graphics::MeshBuffer mMeshBuffer;
	NardaEngine::Graphics::VertexShader mVertexShader;
	NardaEngine::Graphics::PixelShader mPixelShader;

	
};

class CubeShapeState : public ShapeState
{
protected:
	void CreateShape() override;
};


class PyramidShapeState : public ShapeState
{
protected:
	void CreateShape() override;
};

class RectangleShapeState : public ShapeState
{
protected:
	void CreateShape() override;
};
