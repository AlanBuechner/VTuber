#include "Application.h"

int Engine::Application::s_QuitMessage = 0;

namespace Engine
{

	Application::Application()
	{}

	int Engine::Application::Run()
	{

		RendererCommand::Init();

		m_Window1 = new Window(1080, 720, L"VTuber");
		m_Window2 = new Window(1080, 720, L"VTuber2");

		OnCreate();

		while (s_QuitMessage == 0)
		{
			m_Window1->OnUpdate();
			m_Window2->OnUpdate();

			OnUpdate();

			m_Window1->SwapBuffers();
			m_Window2->SwapBuffers();
		}

		OnDestroy();

		return s_QuitMessage;
	}

	void Application::OnCreate()
	{

	}

	void Application::OnUpdate()
	{
		RendererCommand::SetTarget(m_Window1->GetTarget());
		m_Window1->ClearToColor(1.0f, 0.0f, 0.0f);
		RendererCommand::GetGraphics().DrawTestTriangle();

		RendererCommand::SetTarget(m_Window2->GetTarget());
		m_Window2->ClearToColor(0.0f, 1.0f, 0.0f);
		RendererCommand::GetGraphics().DrawTestTriangle();
	}

	void Application::OnDestroy()
	{
		delete m_Window1;
		delete m_Window2;
	}
}