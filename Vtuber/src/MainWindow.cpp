#include "Core/Core.h"
#include "Core/Time.h"
#include "MainWindow.h"
#include "Application.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommand.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"

void MainWindow::OnCreate()
{

	Engine::BufferLayout layout = {
		{Engine::ShaderDataType::Float4, "Position"}
	};

	vb = Engine::VertexBuffer::Create(3 * sizeof(Engine::Mesh::Vertex));
	vb->SetLayout(layout);
	vb->Bind();

	ib = Engine::IndexBuffer::Create(3);
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

	Engine::Mesh::Vertex verts[] = {
		vertices[0].Position * data.rot,
		vertices[1].Position * data.rot,
		vertices[2].Position * data.rot
	};

	vb->SetData((void*)verts, 3*sizeof(Engine::Mesh::Vertex));

	ib->SetData(indices, 3);

	Engine::RendererCommand::DrawIndexed(ib->GetCount());
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}