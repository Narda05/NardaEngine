#include "Precompiled.h"
#include "RenderObject.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
	TextureManager* tm = TextureManager::Get(); 
	tm->RealeaseTexture(diffuseMapId); 
	tm->RealeaseTexture(specMadId);
	tm->RealeaseTexture(normalMapId);
	tm->RealeaseTexture(bumpMapId);
}

void RenderGroup::Initialize(const std::filesystem::path& modelFilePath)
{
	modelId = ModelManager::Get()->LoadModel(modelFilePath);
	const Model* model = ModelManager::Get()->GetModel(modelId);
	ASSERT(model != nullptr, "RenderGroup: model %s did not load", modelFilePath.u8string().c_str());

	for(const Model::MeshData& meshData : model ->meshData)
	{
		RenderObject& renderObject = renderObjects.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if(meshData.materialIndex < model->materialData.size())
		{
			// add materila data
		}
	}
	
}

void RenderGroup::Terminate()
{
	for (auto& renderObject : renderObjects)
	{
		renderObject.Terminate();
	}
	renderObjects.clear();
}