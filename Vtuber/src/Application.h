#pragma once
#include "Core/Window.h"

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

		Window* m_Window1;
		Window* m_Window2;

		static int s_QuitMessage;
	};
}