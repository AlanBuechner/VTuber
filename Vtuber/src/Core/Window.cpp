#include "Core.h"
#include "Window.h"
#include "Application.h"

#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif // PLATFORM_WINDOWS


#include <functional>

std::vector<Engine::Ref<Engine::Window>> Engine::Window::s_Windows;
std::vector<Engine::Window*> Engine::Window::s_WindowsToRemove;

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


	Window::Window(WindowProps props) :
		m_NativeWindow(NativeWindow::CreateNativeWindow(props, this))
	{

	}

	Window::~Window()
	{
		delete &m_NativeWindow;
	}

	void Window::Update()
	{
		m_NativeWindow.PullEvents();

		m_Input.UpdateKeyStates();

		WindowProps props = m_NativeWindow.GetProps();
		RendererCommand::SetViewPort(props.width, props.height, 0, 0);
		m_NativeWindow.GetSwapChain().Bind();

		OnUpdate();
	}

	void Window::Resize(int width, int height)
	{
		m_NativeWindow.Resize(width, height);
	}

	void Window::SwapBuffers()
	{
		m_NativeWindow.SwapBuffers();
	}

	void Window::ClearToColor(float r, float g, float b)
	{
		m_NativeWindow.ClearToColor(r, g, b);
	}

	void Window::CloseWindow()
	{
		OnClose();
		s_WindowsToRemove.push_back(this);
	}

	void Window::UpdateWindows()
	{
		for (uint32_t i = 0; i < s_Windows.size(); i++)
		{
			s_Windows[i]->Update();
		}
	}

	void Window::SwapWindowsBuffers()
	{
		for (uint32_t i = 0; i < s_Windows.size(); i++)
		{
			s_Windows[i]->SwapBuffers();
		}
	}

	void Window::RemoveWindows()
	{
		for(uint32_t wtr = 0; wtr < s_WindowsToRemove.size(); wtr++)
		{
			for (uint32_t w = 0; w < s_Windows.size(); w++)
			{
				if (s_Windows[w].get() == s_WindowsToRemove[wtr]) {
					s_Windows.erase(s_Windows.begin() + w);
					break;
				}
			}
		}
		s_WindowsToRemove.clear();
	}


	NativeWindow& NativeWindow::CreateNativeWindow(Window::WindowProps props, Window* owningWindow)
	{

#ifdef PLATFORM_WINDOWS
		return *(new WindowsWindow(props, owningWindow));
#endif // PLATFORM_WINDOWS

	}

}