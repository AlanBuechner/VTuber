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
		Window(int width, int height, const wchar_t* name);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void OnUpdate();
		void SwapBuffers();
		void ClearToColor(float r, float g, float b);

		wrl::ComPtr<ID3D11RenderTargetView>& GetTarget() { return pTarget; }

	private:
		static LRESULT WINAPI HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		Input m_Input;

	private:
		int Width;
		int Height;
		HWND hWnd;
		wrl::ComPtr<IDXGISwapChain> pSwap;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;
	};
}

#define HWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define HWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())