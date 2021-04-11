#include "MainWindow.h"
#include "Application.h"
#include "Renderer/RendererCommand.h"

void MainWindow::OnCreate()
{

}

void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	Engine::RendererCommand::GetGraphics().DrawTestTriangle();
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}