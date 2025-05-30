#include <NardaEngine/Inc/SpEngine.h>
#include "ShapeState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	NardaEngine::AppConfig config;
	config.appName = L"Hello Cube";

	NardaEngine::App& myApp = NardaEngine::MainApp();
	myApp.AddAppState<ShapeState>("ShapeState");
    myApp.AddAppState<CubeShapeState>("cubeShapeState");
	myApp.AddAppState<PyramidShapeState>("pyramidShapeState");
	myApp.AddAppState<RectangleShapeState>("rectangleShapeState");
	myApp.Run(config);

	return 0;
}