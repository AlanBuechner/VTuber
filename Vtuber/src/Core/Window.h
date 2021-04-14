#pragma once
#include "Platform/Windows/Win.h"
#include "Core.h"
#include "VException.h"
#include "Input/Input.h"
#include "Renderer/Graphics.h"
#include "Renderer/RendererCommand.h"

#include <string>
#include <vector>
#include <memory>

namespace Engine
{

	class NativeWindow;

	class Window
	{
	public:
		class Exception : public VException
		{
		public:
			Exception(int line, const char* file, HRESULT hr);
			virtual const char* what() const override;
			virtual const char* GetType() const override;
			static std::string TranslateErrorCode(HRESULT hr);
			HRESULT GetErrorCode() const;
			std::string GetErrorString() const;
		private:
			HRESULT hr;

		};

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

	protected:
		virtual void OnCreate() {};
		virtual void OnUpdate() {};
		virtual void OnClose() {};

	public:
		Input m_Input;

	private:
		static std::vector<Ref<Window>> s_Windows;
		static std::vector<Window*> s_WindowsToRemove;

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
		virtual wrl::ComPtr<ID3D11RenderTargetView>& GetTarget() = 0;

		virtual void PullEvents() = 0;

		virtual void Resize(int width, int height) = 0;
		virtual void CloseWindow() = 0;

		virtual void SwapBuffers() = 0;
		virtual void ClearToColor(float r, float g, float b) = 0;

		static NativeWindow& CreateNativeWindow(Window::WindowProps props, Window* owningWindow);


	};
}

#define HWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define HWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())