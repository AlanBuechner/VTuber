#include "Application.h"
#include "Core/Time.h"
#include "Renderer/RendererCommand.h"
#include "Util/Performance.h"

int Engine::Application::s_QuitMessage = 0;

namespace Engine
{

	Application::Application()
	{}

	int Engine::Application::Run()
	{

		RendererCommand::Init();

		Window::MakeWindow<MainWindow>(1080, 720, L"VTuber");

		OnCreate();

		while (s_QuitMessage == 0)
		{
			Time::UpdateDeltaTime();
			Window::UpdateWindows();

			OnUpdate();

			Window::SwapWindowsBuffers();
			Window::RemoveWindows();


			DBOUT("FPS: " << Time::GetFPS() << std::endl);
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