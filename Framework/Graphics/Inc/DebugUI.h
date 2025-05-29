#pragma once

namespace NardaEngine::Graphics::DebugUI
{
	enum class Theme
	{
		Classic, 
		Dark,
		Light
	};

	void StaticInitialize(HWND window, bool dockying = false, bool multiViewport = false);
	void StaticTerminate();
	void SetTheme(Theme theme);

	void BeginRender();
	void EndRender();


}