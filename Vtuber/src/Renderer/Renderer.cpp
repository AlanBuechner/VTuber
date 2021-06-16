#include "Renderer/Renderer.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"

glm::mat4 Engine::Renderer::m_ViewProjectionMatrix = glm::mat4(1.0f);

namespace Engine
{
	void Renderer::StartScene(const Ref<Camera>& camera)
	{
		m_ViewProjectionMatrix = camera->GetProjectionMatrix();
	}

	void Renderer::StartScene(const Ref<glm::mat4>& viewPorjectionMatrix)
	{
		m_ViewProjectionMatrix = *viewPorjectionMatrix.get();
	}

	void Renderer::EndScene()
	{
		m_ViewProjectionMatrix = glm::mat4(1.0f);
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		mesh->Bind();
		shader->Bind();
		shader->SetBuffer("World", (void*)&m_ViewProjectionMatrix);
		shader->SetBuffer("Model", (const void*)&transform);
		uint32_t count = mesh->GetIndexBuffer()->GetCount();
		RendererCommand::DrawIndexed(count);
	}




}