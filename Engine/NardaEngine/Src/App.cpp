#pragma once
#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace NardaEngine;
using namespace NardaEngine::Core;
using namespace NardaEngine::Graphics;

void App::Run(const AppConfig& config)
{
	LOG("App Started");
	//intializa everything
	Window myWindow; 
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);
	auto handler = myWindow.GetWindowHandle();
	GraphicsSystem::StaticInitialize(handler, false);

	//last steo before running
	ASSERT(mCurrentState != nullptr, "App: need an app state to run");
	mCurrentState->Initialize();

	//process updates
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessages();
		if (!myWindow.IsActive())
		{
			Quit();
			continue; 
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
		if (deltaTime < 0.5f) // primarily for handling breakpoints
#endif
		{
			mCurrentState->Update(deltaTime);
		}

		GraphicsSystem* gs = GraphicsSystem::Get();
		gs->BeginRender();
		mCurrentState->Render();
		gs->EndRender();
	}


	//Terminate everything
	LOG("App Quit");
	mCurrentState->Terminate();

	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}