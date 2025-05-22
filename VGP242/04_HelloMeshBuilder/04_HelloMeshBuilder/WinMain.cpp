#include <NardaEngine/Inc/SpEngine.h>
#include "ShapeState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	NardaEngine::AppConfig config;
	config.appName = L"Hello Mesh Builder";

	NardaEngine::App& myApp = NardaEngine::MainApp();
	myApp.AddAppState<ShapeState>("ShapeState");
	myApp.Run(config);


	return 0;
}