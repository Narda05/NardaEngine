#include "Precompiled.h"
#include "RenderObject.h"

using namespace NardaEngine;
using namespace NardaEngine::Graphics;

void RenderObject::Terminate()
{
	meshbuffer.Terminate();
}