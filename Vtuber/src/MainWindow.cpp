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

	m_Camera = Engine::Camera::Create(Engine::Camera::ProjectionType::Perspective, glm::radians(45.0f), 0.01, 100.0f, GetAspect());

	m_Mesh = Engine::Mesh::Create("Assets/Models/nanosuit.obj");

	m_BlueLight.position = { 0.0f, 0.0f, 2.0f };
	m_BlueLight.color = { 0.0f, 0.0f, 1.0f };
	m_BlueLight.diffuseIntensity = 5.0f;
	m_BlueLight.attQuad = 0.5f;

	m_GreenLight.position = { 0.0f, 0.0f, 2.0f };
	m_GreenLight.color = { 0.0f, 1.0f, 0.0f };
	m_GreenLight.diffuseIntensity = 5.0f;
	m_GreenLight.attQuad = 0.5f;
}

glm::vec3 camPos = { 0.0f, 0.0f, 4.0f };
float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	const float deltaTime = Time::GetDeltaTime();
	a += deltaTime;

	if (m_Input.GetKeyDown('A'))
		camPos.x -= deltaTime;
	if (m_Input.GetKeyDown('D'))
		camPos.x += deltaTime;
	if (m_Input.GetKeyDown('W'))
		camPos.z -= deltaTime;
	if (m_Input.GetKeyDown('S'))
		camPos.z += deltaTime;
	if (m_Input.GetKeyDown(VK_SPACE))
		camPos.y += deltaTime;
	if (m_Input.GetKeyDown(VK_CONTROL))
		camPos.y -= deltaTime;

	/*glm::mat4 rot = glm::rotate(glm::mat4(1.0f), a, { 0.0f, 0.0f, 1.0f }) * 
					glm::rotate(glm::mat4(1.0f), a/2, { 1.0f, 0.0f, 0.0f }) *
					glm::rotate(glm::mat4(1.0f), a*2, { 0.0f, 1.0f, 0.0f });*/

	const float scale = 0.2f;
	glm::mat4 scalemat = glm::scale(glm::mat4(1.0f), { scale, scale, scale });

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 1.0f, -1.5f, 0.0f }) * scalemat;
	glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), { -1.0f, -1.5f, 0.0f }) * scalemat;

	m_Camera->SetAspect(GetAspect());

	glm::mat4 vpMatrix = m_Camera->GetProjectionMatrix() * glm::inverse(glm::translate(glm::mat4(1.0f), camPos));

	m_BlueLight.position = { glm::cos(a), glm::sin(a), 0.0f };
	m_GreenLight.position = { -glm::cos(a), -glm::sin(a), 0.0f };

	Engine::Renderer::StartScene(vpMatrix);
	Engine::Renderer::SubmitLight(m_BlueLight);
	Engine::Renderer::SubmitLight(m_GreenLight);
	Engine::Renderer::Submit(m_Mesh, shader, transform);
	Engine::Renderer::Submit(m_Mesh, shader, transform2);
	Engine::Renderer::EndScene();
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}