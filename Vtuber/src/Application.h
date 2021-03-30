#pragma once
#include "Window.h"

namespace Engine
{
	class Application
	{
	public:
		Application();

		int Run();

		static void Quit(int message = -1) { s_QuitMessage = message; }

	protected:
		void OnCreate();
		void OnUpdate();
		void OnDestroy();
		
	protected:

		Window m_Window;

		static int s_QuitMessage;
	};
}