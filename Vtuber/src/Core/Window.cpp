#include "Window.h"
#include "Application.h"

#include <functional>

Engine::Window::WindowClass Engine::Window::WindowClass::wndClass;

namespace Engine
{

	Window::Exception::Exception(int line, const char* file, HRESULT hr) :
		VException(line, file),
		hr(hr)
	{
	}

	const char* Window::Exception::what() const
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorString() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Window::Exception::GetType() const
	{
		return "Window Exeption";
	}

	std::string Window::Exception::TranslateErrorCode(HRESULT hr)
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	HRESULT Window::Exception::GetErrorCode() const
	{
		return hr;
	}

	std::string Window::Exception::GetErrorString() const
	{
		return TranslateErrorCode(hr);
	}



	Window::WindowClass::WindowClass() :
		hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = Window::HandleEventSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = nullptr;
		wc.hIconSm = nullptr;
		wc.hbrBackground = nullptr;
		wc.hCursor = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();

		RegisterClassEx(&wc);
	}

	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}

	Window::Window(int width, int height, const wchar_t* name) :
		Width(width), Height(height)
	{
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;
		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		{
			throw HWND_LAST_EXCEPT();
		}

		hWnd = CreateWindow(WindowClass::GetName(), (LPCWSTR)name,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this);

		if (hWnd == nullptr)
		{
			throw HWND_LAST_EXCEPT();
		}

		ShowWindow(hWnd, SW_SHOW);

		Graphics& graphics = RendererCommand::GetGraphics();

		graphics.CreateSwapChainAndRenderTarget(hWnd, pSwap, pTarget);

		RendererCommand::SetViewPort(width, height, 0, 0);
	}

	Window::~Window()
	{
		DestroyWindow(hWnd);
	}

	void Window::OnUpdate()
	{
		MSG msg;
		BOOL gResult = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		if (gResult != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_Input.UpdateKeyStates();
	}

	void Window::SwapBuffers()
	{
		pSwap->Present(1u, 0u);
	}

	void Window::ClearToColor(float r, float g, float b)
	{
		Graphics& graphics = RendererCommand::GetGraphics();
		const float color[] = { r,g,b,1.0f };
		graphics.GetContext()->ClearRenderTargetView(pTarget.Get(), color);
	}

	LRESULT WINAPI Window::HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); // set user data on the window to contain the Window class
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleEventThunk)); // change the event handler to handle event thunk

			return pWnd->HandleEvent(hWnd, msg, wParam, lParam); // handle the event
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI Window::HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleEvent(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
		switch (msg)
		{
		// keyboard events
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if(!(lParam & 0x40000000) || m_Input.AutorepeatEnabled())
				m_Input.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			m_Input.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			m_Input.OnChar(static_cast<unsigned char>(wParam));
			break;

		// mouse
		case WM_MOUSEMOVE:
			POINTS pt = MAKEPOINTS(lParam);
			m_Input.OnMouseMove(pt.x, pt.y);
			break;

		case WM_LBUTTONDOWN:
			m_Input.OnMousePressed(Input::LMouse);
			break;
		case WM_RBUTTONDOWN:
			m_Input.OnMousePressed(Input::RMouse);
			break;
		case WM_MBUTTONDOWN:
			m_Input.OnMousePressed(Input::MMouse);
			break;

		case WM_LBUTTONUP:
			m_Input.OnMouseReleased(Input::LMouse);
			break;
		case WM_RBUTTONUP:
			m_Input.OnMouseReleased(Input::RMouse);
			break;
		case WM_MBUTTONUP:
			m_Input.OnMouseReleased(Input::MMouse);
			break;


		// window events
		case WM_SIZE:
			//unsigned int width = LOWORD(lParam);
			//unsigned int height = HIWORD(lParam);
			break;
		case WM_KILLFOCUS:
			m_Input.ClearKeyStates();
			break;
		case WM_CLOSE:
			Application::Quit(69);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }


	

}