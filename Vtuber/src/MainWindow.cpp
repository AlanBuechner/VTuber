#include "MainWindow.h"
#include "Application.h"

#include "Renderer/RendererAPI.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"
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

	float a = 3.1415926543f;
	struct CBData {
		struct {
			float elements[4][4];
		};
	};
	const CBData data = {
		{
			 cos(a),	sin(a), 0.0f, 0.0f,
			-sin(a),	cos(a), 0.0f, 0.0f,
			 0.0f,		0.0f,	1.0f, 0.0f,
			 0.0f,		0.0f,	0.0f, 1.0f
		}
	};
	Engine::Ref<Engine::ConstentBuffer> cb = Engine::ConstentBuffer::Create((void*)&data, sizeof(data));

	shader->SetConstantBuffer(0u, *cb.get());
}

void MainWindow::OnUpdate()
{
	ClearToColor(1.0f, 0.0f, 0.0f);
	//((Engine::DirectX11RendererAPI*)Engine::RendererAPI::Get())->DrawTestTriangle();

	

	Engine::RendererCommand::DrawIndexed(ib->GetCount());

}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}