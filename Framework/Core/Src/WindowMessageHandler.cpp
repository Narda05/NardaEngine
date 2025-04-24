#include "Precompiled.h"
#include "WindowMessageHandler.h"
#include "DebugUtil.h"

using namespace NardaEngine;
using namespace NardaEngine::Core;

void WindowMessageHandler::Hook(HWND window, Callback cb)
{
	mWindow = window;
	mPreviusCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);

}

void WindowMessageHandler::Unhook()
{
	
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviusCallback);
	mWindow = nullptr;
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(mPreviusCallback != nullptr, "WindowMessageHandler: no callbck is hooked");
	return CallWindowProcA((WNDPROC)mPreviusCallback, window, message, wParam, lParam);
}
