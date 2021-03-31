#include "Application.h"

int Engine::Application::s_QuitMessage = 0;

namespace Engine
{

	Application::Application() :
		m_Window(Window(1080, 720, L"VTuber"))
	{}

	int Engine::Application::Run()
	{
		OnCreate();

		while (s_QuitMessage == 0)
		{
			m_Window.OnUpdate();

			OnUpdate();

			m_Window.SwapBuffers();
		}

		OnDestroy();

		return s_QuitMessage;
	}

	void Application::OnCreate()
	{

	}

	void Application::OnUpdate()
	{
		m_Window.ClearToColor(1.0f, 0.0f, 0.0f);
	}

	void Application::OnDestroy()
	{

	}
}