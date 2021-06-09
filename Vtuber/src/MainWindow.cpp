#include "Core/Core.h"
#include "Core/Time.h"
#include "MainWindow.h"
#include "Application.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommand.h"

struct CBData {
	struct {
		glm::mat4 rot;
		float val = 0.5f;
	};
};

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

	ib = Engine::IndexBuffer::Create(indices, 3);
	ib->Bind();

	shader = Engine::Shader::Create();

	shader->LoadVertexShader(L"VertexShader.vertex.cso");
	shader->LoadPixleShader(L"PixelShader.pixel.cso");
	shader->SetInputLayout(layout);

	shader->Bind();

	m_NativeWindow.GetSwapChain().SetVSync(true);
}

float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	float deltat = Time::GetDeltaTime();
	a += deltat;
	//a += 0.01666;
	const CBData data = {
		glm::rotate(glm::mat4(1.0f), a, {0.0f, 0.0f, 1.0f})
	};

	shader->SetBuffer("CBuff", (void*)&data);

	Engine::RendererCommand::DrawIndexed(ib->GetCount());
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}