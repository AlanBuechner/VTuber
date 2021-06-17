#include "Renderer/Renderer.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"

glm::mat4 Engine::Renderer::m_ViewProjectionMatrix = glm::mat4(1.0f);
Engine::Renderer::Lights Engine::Renderer::m_Lights;

namespace Engine
{
	void Renderer::StartScene(const Ref<Camera>& camera)
	{
		m_ViewProjectionMatrix = camera->GetProjectionMatrix();
		m_Lights.numLights = 0;
	}

	void Renderer::StartScene(const glm::mat4& viewPorjectionMatrix)
	{
		m_ViewProjectionMatrix = viewPorjectionMatrix;
		m_Lights.numLights = 0;
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
		shader->SetBuffer("Model", (void*)&transform);
		shader->SetBuffer("Lights", (void*)&m_Lights);
		uint32_t count = mesh->GetIndexBuffer()->GetCount();
		RendererCommand::DrawIndexed(count);
	}

	void Renderer::SubmitLight(const PointLight& light)
	{
		if (m_Lights.numLights == MAX_LIGHTS) {
			DBOUT("max lights reached");
			return;
		}

		m_Lights.lights[m_Lights.numLights++] = light;
	}




}