#pragma once
#include "Core.h"
#include "VException.h"
#include "Input/Input.h"
#include "Platform/Windows/Win.h"

#include "Renderer/SwapChain.h"

#include "Platform/WindowExeption.h"

#include <string>
#include <vector>
#include <memory>

/*
TODO : fix vsync with multiple windows
*/

namespace Engine
{

	class NativeWindow;

	class Window
	{
	public:

		struct WindowProps
		{
			int width, height;
			std::wstring name;
		};

	public:
		Window(WindowProps props);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void Update();

		void Resize(int width, int height);

		void SwapBuffers();
		void ClearToColor(float r, float g, float b);

		void CloseWindow();

		float GetAspect();

	protected:
		virtual void OnCreate() {};
		virtual void OnUpdate() {};
		virtual void OnClose() {};

	public:
		Input m_Input;

	private:
		static std::vector<Ref<Window>> s_Windows;
		static std::vector<Window*> s_WindowsToRemove;

	protected:
		NativeWindow& m_NativeWindow;

	public:

		template<typename T>
		static Ref<T> MakeWindow(int width, int height, std::wstring name)
		{
			WindowProps props{ 0 };
			props.width = width;
			props.height = height;
			props.name = name;
			Ref<T> window = std::make_shared<T>(props);
			s_Windows.push_back(window);
			((Window*)window.get())->OnCreate();
			return window;
		}

		static void UpdateWindows();
		static void SwapWindowsBuffers();
		static void RemoveWindows();
	};

	class NativeWindow
	{
	public:
		NativeWindow(Window::WindowProps props, Window* owningWindow) {};
		virtual ~NativeWindow() {};

		NativeWindow(const NativeWindow&) = delete;
		NativeWindow& operator=(const NativeWindow&) = delete;

		virtual Window::WindowProps GetProps() = 0;

		virtual void PullEvents() = 0;

		virtual void Resize(int width, int height) = 0;
		virtual void CloseWindow() = 0;

		virtual void SwapBuffers() = 0;
		virtual void ClearToColor(float r, float g, float b) = 0;

		virtual SwapChain& GetSwapChain() = 0;

		static NativeWindow& CreateNativeWindow(Window::WindowProps props, Window* owningWindow);
	};
}