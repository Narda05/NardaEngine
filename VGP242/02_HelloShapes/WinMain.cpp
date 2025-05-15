#include <NardaEngine/Inc/SpEngine.h>
#include "ShapeState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	NardaEngine::AppConfig config;
	config.appName = L"Hello Shapes";

	NardaEngine::App& myApp = NardaEngine::MainApp();
	myApp.AddAppState<ShapeState>("ShapeState");
	myApp.AddAppState<TriangleShapeState>("TriangleShapeState");
	myApp.AddAppState<HeartShapeState>("HeartShapeState");
	myApp.AddAppState<FishShapeState>("FishShapeState");
	myApp.AddAppState<StarShapeState>("StarShapeState");
	myApp.Run(config);

	return 0;
}