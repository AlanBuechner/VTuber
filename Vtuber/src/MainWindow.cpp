#include "Core/Core.h"
#include "Core/Time.h"
#include "MainWindow.h"
#include "Application.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommand.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"

void MainWindow::OnCreate()
{
	Engine::ShaderSource src;
	src.VetexShader = L"VertexShader.vertex.cso";
	src.PixelShader = L"PixelShader.pixel.cso";

	shader = Engine::Shader::Create(src);
	shader->Bind();

	m_NativeWindow.GetSwapChain().SetVSync(true);

	m_Camera = Engine::Camera::Create(Engine::Camera::ProjectionType::Orthographic, 2, -1.0f, 1.0f, GetAspect());

	m_Texture = Engine::Texture2D::Create(1,1);
	m_Texture->Bind(0);
}

float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	a += Time::GetDeltaTime();

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), a, { 0.0f, 0.0f, 1.0f });
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });// *rot;

	m_Camera->SetAspect(GetAspect());

	Engine::Renderer::StartScene(m_Camera);
	Engine::Renderer::Submit(m_Mesh, shader, transform);
	Engine::Renderer::EndScene();
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}