#include "Renderer2D.h"

#include "RendererAPI.h"
#include "RendererCommand.h"

Engine::Ref<Engine::Shader> Engine::Renderer2D::m_Shader;

struct SceneData
{
	Engine::Ref<Engine::Mesh::SubMesh> quadMesh;
} data;

namespace Engine
{
	void Renderer2D::Init()
	{
		Mesh::Vertex verts[]{
			{ {-0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
			{ {-0.5f,  0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
			{ { 0.5f,  0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
		};

		uint32_t indeces[]{
			0, 1, 2,
			2, 3, 0
		};

		data.quadMesh = Mesh::SubMesh::Create(verts, 4, indeces, 6);
	}

	void Renderer2D::BeginScene(const Ref<Camera>& camera)
	{
		BeginScene(camera->GetProjectionMatrix());
	}

	void Renderer2D::BeginScene(const glm::mat4& viewPorjectionMatrix)
	{
		m_Shader->SetBuffer("World", (void*)&viewPorjectionMatrix);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(glm::mat4& transform, glm::vec4 color, Ref<Texture2D>& texture)
	{
		data.quadMesh->Bind();
		texture->Bind(0);
		m_Shader->SetBuffer("Model", (void*)&transform);
		RendererCommand::DrawIndexed(data.quadMesh->GetIndexBuffer()->GetCount());
	}

}