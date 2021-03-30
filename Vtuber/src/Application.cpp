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
		}

		OnDestroy();

		return s_QuitMessage;
	}

	void Application::OnCreate()
	{
	}

	void Application::OnUpdate()
	{

	}

	void Application::OnDestroy()
	{

	}
}