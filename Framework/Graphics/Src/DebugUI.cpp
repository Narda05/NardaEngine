#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicsSystem.h"

#include<ImGui/Inc/imgui_impl_dx11.h> 
#include<ImGui/Inc/imgui_impl_win32.h>

using namespace NardaEngine;
using namespace NardaEngine::Graphics;
using namespace NardaEngine::Core;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT msg)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return true; 
		default:
			break;
		}
		return false;
	}
	bool IsKeyboardMessage(UINT msg)
	{
		switch (msg)
		{
		case WM_CHAR:
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			return true;
		default:
			break;
		}
		return false;
	}
	LRESULT CALLBACK DebugUIMessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		//does imui want to cpature the mouse and is it a mouse message
		if (io.WantCaptureMouse && IsMouseInput(msg))
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam); // let imgui handle it
		}

		//does imgui want to capture key input and is it 
		if (io.WantCaptureKeyboard && IsKeyboardMessage(msg))
		{
			return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam); // let imgui handle it
		}

		LRESULT result = ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		if (result != 0)
		{
			return result; // let imgui handle it
		}

		return sWindowMessageHandler.ForwardMessage(hwnd, msg, wParam, lParam);
	}
}

void DebugUI::StaticInitialize(HWND window, bool dockying, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	if (dockying)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}
	if (multiViewport)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);

	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);

}
void DebugUI::StaticTerminate()
{
	sWindowMessageHandler.Unhook();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
void DebugUI::SetTheme(Theme theme) 
{
	switch (theme)
	{
		case Theme::Classic:
		{
			ImGui::StyleColorsClassic();
				break;
		}
		case Theme::Dark:
		{
			ImGui::StyleColorsDark();
			break;
		}
		case Theme::Light:
		{
			ImGui::StyleColorsLight();
			break;
		}
		default:
		{
			ASSERT(false, "DebugUI: invalide theme");
			break;
		}
	}
		
}

void DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}