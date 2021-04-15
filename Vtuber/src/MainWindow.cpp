#include "MainWindow.h"
#include "Application.h"

#include "Renderer/RendererAPI.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"
#include "Renderer/RendererCommand.h"

void MainWindow::OnCreate()
{

}

void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	((Engine::DirectX11RendererAPI*)Engine::RendererAPI::Get())->DrawTestTriangle();
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}