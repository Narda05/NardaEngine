#pragma once
#include "Precompiled.h"
#include "App.h"

using namespace NardaEngine;
using namespace NardaEngine::Core;

void ::App::Run()
{
	LOG("App Started");

	mRunning = true;
	while (mRunning)
	{
		LOG("Running");
	}

	LOG("App Quit");
}

void App::Quit()
{
	mRunning = false;
}
