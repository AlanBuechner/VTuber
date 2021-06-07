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

	ib = Engine::IndexBuffer::Create(indices, 3);
	ib->Bind();

	shader = Engine::Shader::Create();

	shader->LoadVertexShader(L"VertexShader.cso");
	shader->LoadPixleShader(L"PixelShader.cso");
	shader->SetInputLayout(layout);

	shader->Bind();
}

float a = 0.0f;
void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);

	a += 0.01666;
	struct CBData {
		struct {
			glm::mat4 rot;
		};
	};
	const CBData data = {
		glm::rotate(glm::mat4(1.0f), a, {0.0f, 0.0f, 1.0f})
	};
	Engine::Ref<Engine::ConstentBuffer> cb = Engine::ConstentBuffer::Create((void*)&data, sizeof(data));

	shader->SetConstantBuffer(0u, *cb.get());

	Engine::RendererCommand::DrawIndexed(ib->GetCount());
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}