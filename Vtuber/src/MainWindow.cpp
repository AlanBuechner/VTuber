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

	m_Camera = Engine::Camera::Create(Engine::Camera::ProjectionType::Perspective, 2, 0.01, 100.0f, GetAspect());

	//m_Texture = Engine::Texture2D::Create(1,1);
	m_Texture = Engine::Texture2D::Create("Assets/Textures/bunnie.jpg");
	m_Texture->Bind(0);

	m_BlueLight.position = { 0.0f, 0.0f, 2.0f };
	m_BlueLight.color = { 0.0f, 0.0f, 1.0f };
	m_BlueLight.diffuseIntensity = 5.0f;
	m_BlueLight.attQuad = 0.5f;

	m_RedLight.position = { 0.0f, 0.0f, 2.0f };
	m_RedLight.color = { 1.0f, 0.0f, 0.0f };
	m_RedLight.diffuseIntensity = 5.0f;
	m_RedLight.attQuad = 0.5f;
}

float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	a += Time::GetDeltaTime();

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), a, { 0.0f, 0.0f, 1.0f }) * 
					glm::rotate(glm::mat4(1.0f), a/2, { 1.0f, 0.0f, 0.0f }) *
					glm::rotate(glm::mat4(1.0f), a*2, { 0.0f, 1.0f, 0.0f });
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 1.0f, 0.0f, 0.0f }) * rot;
	glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), { -1.0f, 0.0f, 0.0f }) * rot;

	m_Camera->SetAspect(GetAspect());

	glm::mat4 vpMatrix = m_Camera->GetProjectionMatrix() * glm::inverse(glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 1.5f }));

	m_BlueLight.position = { glm::cos(a), glm::sin(a), 0.0f };
	m_RedLight.position = { -glm::cos(a), -glm::sin(a), 0.0f };

	Engine::Renderer::StartScene(vpMatrix);
	Engine::Renderer::SubmitLight(m_BlueLight);
	Engine::Renderer::SubmitLight(m_RedLight);
	Engine::Renderer::Submit(m_Mesh, shader, transform);
	Engine::Renderer::Submit(m_Mesh, shader, transform2);
	Engine::Renderer::EndScene();
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}