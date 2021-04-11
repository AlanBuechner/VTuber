#pragma once
#include "Win.h"
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


	private:
		class WindowClass
		{
		public:
			static const LPCWSTR GetName() noexcept { return wndClassName; }
			static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

		private:
			WindowClass();
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			static constexpr const LPCWSTR wndClassName = TEXT("Vtuber");
			static WindowClass wndClass;
			HINSTANCE hInst;
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

		void SwapBuffers();
		void ClearToColor(float r, float g, float b);

		void CloseWindow();

		wrl::ComPtr<ID3D11RenderTargetView>& GetTarget() { return pTarget; }

	protected:
		virtual void OnUpdate() {};
		virtual void OnClose() {};

	private:
		static LRESULT WINAPI HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		Input m_Input;

	private:
		static std::vector<Ref<Window>> s_Windows;
		static std::vector<Window*> s_WindowsToRemove;

		WindowProps m_Props;
		HWND hWnd;
		wrl::ComPtr<IDXGISwapChain> pSwap;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;

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
			return window;
		}

		static void UpdateWindows();
		static void SwapWindowsBuffers();
		static void RemoveWindows();
	};
}

#define HWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define HWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())