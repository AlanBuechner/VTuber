#pragma once
#include "Core/Window.h"
#include "MainWindow.h"

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

		static int s_QuitMessage;
	};
}