#include "Precompiled.h"
#include "SpEngine.h"

NardaEngine::App& NardaEngine::MainApp()
{
	static App sApp;
	return sApp;
}