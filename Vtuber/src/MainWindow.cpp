#include "Core/Core.h"
#include "Core/Time.h"
#include "MainWindow.h"
#include "Application.h"

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

	m_Mesh->UpdateVertexBuffer(verts, 3);

	m_Camera->SetAspect(GetAspect());

	shader->SetBuffer("World", (void*)&m_Camera->GetProjectionMatrix());

	Engine::RendererCommand::DrawIndexed(m_Mesh->GetIndexBuffer()->GetCount());
}

void MainWindow::OnClose()
{
	Engine::Application::Quit(69);
}