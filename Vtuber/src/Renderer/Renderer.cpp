#include "Renderer/Renderer.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"

glm::mat4 Engine::Renderer::m_ViewProjectionMatrix = glm::mat4(1.0f);
Engine::Renderer::Lights Engine::Renderer::m_Lights;
std::list<Engine::Renderer::RenderObject> Engine::Renderer::m_ObjectsToRender;
std::list<std::list<Engine::Renderer::RenderObject>::const_iterator> Engine::Renderer::m_ShaderStartItorator;

namespace Engine
{
	void Renderer::StartScene(const Ref<Camera>& camera)
	{
		StartScene(camera->GetProjectionMatrix());
	}

	void Renderer::StartScene(const glm::mat4& viewPorjectionMatrix)
	{
		m_ViewProjectionMatrix = viewPorjectionMatrix;
		m_Lights.numLights = 0;
		m_ObjectsToRender.clear();
		m_ShaderStartItorator.clear();
	}

	void Renderer::EndScene()
	{
		RenderObject* last = nullptr;
		for (auto o : m_ObjectsToRender)
		{
			o.mesh->Bind();
			if (!last || o.shader != last->shader)
			{
				o.shader->Bind();
				o.shader->SetBuffer("World", (void*)&m_ViewProjectionMatrix);
				o.shader->SetBuffer("Lights", (void*)&m_Lights);
			}
			o.shader->SetBuffer("Model", (void*)&o.transform);
			uint32_t count = o.mesh->GetIndexBuffer()->GetCount();
			RendererCommand::DrawIndexed(count);

			last = &o;
		}
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		for(auto i : m_ShaderStartItorator)
		{
			if ((i)->shader == shader) 
			{
				i = m_ObjectsToRender.insert(i, { mesh, shader, transform });
				return;
			}
		}

		// if we cant find the shader
		m_ObjectsToRender.push_back({ mesh, shader, transform });
		m_ShaderStartItorator.push_back(--m_ObjectsToRender.end());

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