#pragma once
#include "Precompiled.h"
#include "App.h"
#include "AppState.h"


using namespace NardaEngine;
using namespace NardaEngine::Core;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Input;

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
	InputSystem::StaticInitialize(handler);

	//last steo before running
	ASSERT(mCurrentState != nullptr, "App: need an app state to run");
	mCurrentState->Initialize();

	//process updates

	InputSystem* input = InputSystem::Get();
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessages();
		input->Update();
		if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
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

	InputSystem::StaticTerminate();
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