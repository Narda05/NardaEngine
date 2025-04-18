#pragma once
#include "Common.h"

#include "App.h"

namespace NardaEngine
{
	class App final
	{
	public:
		void Run(); 
		void Quit();
	private:
		bool mRunning = false;
	};
}