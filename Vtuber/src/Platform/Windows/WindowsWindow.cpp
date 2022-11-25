#include "WindowsWindow.h"
#include "Renderer/RendererCommand.h"

#include "Platform/WindowExeption.h"

Engine::WindowsWindow::WindowClass Engine::WindowsWindow::WindowClass::wndClass;

namespace Engine
{

	// window expetion

	WindowException::WindowException(int line, const char* file, HRESULT hr) :
		VException(line, file),
		hr(hr)
	{
	}

	const char* WindowException::what() const
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorString() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* WindowException::GetType() const
	{
		return "Window Exeption";
	}

	std::string WindowException::TranslateErrorCode(HRESULT hr)
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

	HRESULT WindowException::GetErrorCode() const
	{
		return hr;
	}

	std::string WindowException::GetErrorString() const
	{
		return TranslateErrorCode(hr);
	}

	// window class

	WindowsWindow::WindowClass::WindowClass() :
		hInst(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowsWindow::HandleEventSetup;
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

	WindowsWindow::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}

	// windows window

	WindowsWindow::WindowsWindow(Window::WindowProps props, Window* owningWindow) :
		NativeWindow::NativeWindow(props, owningWindow), m_OwningWindow(owningWindow), m_Props(props), m_Swap(SwapChain::CreateSwapChain())
	{
		RECT wr;
		wr.left = 100;
		wr.right = props.width + wr.left;
		wr.top = 100;
		wr.bottom = props.height + wr.top;

		DWORD style = WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		if (AdjustWindowRect(&wr, style, FALSE) == 0)
		{
			throw HWND_LAST_EXCEPT();
		}

		hWnd = CreateWindow(WindowClass::GetName(), (LPCWSTR)props.name.c_str(),
			style,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this);

		if (hWnd == nullptr)
		{
			throw HWND_LAST_EXCEPT();
		}

		ShowWindow(hWnd, SW_SHOW);

		m_Swap.Init((void*)hWnd, props.width, props.height);

		RendererCommand::SetViewPort(props.width, props.height, 0, 0);
	}

	WindowsWindow::~WindowsWindow()
	{
		CloseWindow();
	}

	void WindowsWindow::PullEvents()
	{
		MSG msg;
		BOOL gResult = PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
		if (gResult != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::Resize(int width, int height)
	{
		m_Props.width = width;
		m_Props.height = height;

		SetWindowPos(hWnd, nullptr, 0, 0, width, height, 0);
	}

	void WindowsWindow::CloseWindow()
	{
		DestroyWindow(hWnd);
	}

	void WindowsWindow::SwapBuffers()
	{
		m_Swap.SwapBuffers();
	}

	void WindowsWindow::ClearToColor(float r, float g, float b)
	{
		m_Swap.ClearColor(r, g, b);
	}

	LRESULT WINAPI WindowsWindow::HandleEventSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* const pWnd = static_cast<WindowsWindow*>(pCreate->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); // set user data on the window to contain the Window class
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowsWindow::HandleEventThunk)); // change the event handler to handle event thunk

			return pWnd->HandleEvent(hWnd, msg, wParam, lParam); // handle the event
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI WindowsWindow::HandleEventThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* const pWnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleEvent(hWnd, msg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Input& input = m_OwningWindow->m_Input;

		switch (msg)
		{
			// keyboard events
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (!(lParam & 0x40000000) ||input.AutorepeatEnabled())
				input.OnKeyPressed(static_cast<unsigned char>(wParam));
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			input.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			input.OnChar(static_cast<unsigned char>(wParam));
			break;

			// mouse
		case WM_MOUSEMOVE:
			POINTS pt = MAKEPOINTS(lParam);
			input.OnMouseMove(pt.x, pt.y);
			break;

		case WM_LBUTTONDOWN:
			input.OnMousePressed(Input::LMouse);
			break;
		case WM_RBUTTONDOWN:
			input.OnMousePressed(Input::RMouse);
			break;
		case WM_MBUTTONDOWN:
			input.OnMousePressed(Input::MMouse);
			break;

		case WM_LBUTTONUP:
			input.OnMouseReleased(Input::LMouse);
			break;
		case WM_RBUTTONUP:
			input.OnMouseReleased(Input::RMouse);
			break;
		case WM_MBUTTONUP:
			input.OnMouseReleased(Input::MMouse);
			break;


			// window events
		case WM_SIZE:
		{
			m_Props.width = LOWORD(lParam);
			m_Props.height = HIWORD(lParam);

			m_Swap.Resize(m_Props.width, m_Props.height);
			break;
		}
		case WM_KILLFOCUS:
			input.ClearKeyStates();
			break;
		case WM_CLOSE:
			m_OwningWindow->CloseWindow();
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}