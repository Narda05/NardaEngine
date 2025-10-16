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
}