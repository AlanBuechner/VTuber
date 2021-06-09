#include "Core/Core.h"
#include "Core/Time.h"
#include "MainWindow.h"
#include "Application.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommand.h"

void MainWindow::OnCreate()
{

	const Engine::Mesh::Vertex vertices[] = {
			{ 0.0f,  0.5f, 0.0f},
			{ 0.5f, -0.5f, 0.0f},
			{-0.5f, -0.5f, 0.0f}
	};

	const uint32_t indices[] = {
		0, 1, 2
	};

	Engine::BufferLayout layout = {
		{Engine::ShaderDataType::Float3, "Position"}
	};

	vb = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout(layout);
	vb->Bind();

	ib = Engine::IndexBuffer::Create(indices, sizeof(indices));
	ib->Bind();

	Engine::ShaderSource src;
	src.VetexShader = L"VertexShader.vertex.cso";
	src.PixelShader = L"PixelShader.pixel.cso";

	shader = Engine::Shader::Create(src);
	shader->Bind();

	m_NativeWindow.GetSwapChain().SetVSync(true);
}

float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	a += Time::GetDeltaTime();

	struct CBData {
		glm::mat4 rot;
		float val = 0.5f;
	};
	const CBData data = {
		glm::rotate(glm::mat4(1.0f), a, {0.0f, 0.0f, 1.0f}),
		0.5f
	};

	shader->SetBuffer("CBuff", (void*)&data);

	Engine::RendererCommand::DrawIndexed(ib->GetCount());
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}