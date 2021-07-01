#include "Renderer/Renderer.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"

glm::mat4 Engine::Renderer::s_ViewProjectionMatrix = glm::mat4(1.0f);
Engine::Renderer::Lights Engine::Renderer::s_Lights;
std::list<Engine::Renderer::RenderObject> Engine::Renderer::s_ObjectsToRender;
std::list<std::list<Engine::Renderer::RenderObject>::const_iterator> Engine::Renderer::s_ShaderStartItorator;
Engine::Ref<Engine::Texture2D> Engine::Renderer::s_WhiteTexture;

namespace Engine
{
	void Renderer::Init()
	{
		s_WhiteTexture = Texture2D::Create(1,1);

		Renderer2D::Init();
	}

	void Renderer::BeginScene(const Ref<Camera>& camera)
	{
		BeginScene(camera->GetProjectionMatrix());
	}

	void Renderer::BeginScene(const glm::mat4& viewPorjectionMatrix)
	{
		s_ViewProjectionMatrix = viewPorjectionMatrix;
		s_Lights.numLights = 0;
		s_ObjectsToRender.clear();
		s_ShaderStartItorator.clear();
	}

	void Renderer::EndScene()
	{
		RenderObject* last = nullptr;
		for (auto o : s_ObjectsToRender)
		{
			if (!last || o.shader != last->shader)
			{
				o.shader->Bind();
				o.shader->SetBuffer("World", (void*)&s_ViewProjectionMatrix);
				o.shader->SetBuffer("Lights", (void*)&s_Lights);
			}
			DrawMesh(o.mesh, o.shader, o.transform);

			last = &o;
		}
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		for(auto i : s_ShaderStartItorator)
		{
			if ((i)->shader == shader)
			{
				i = s_ObjectsToRender.insert(i, { mesh, shader, transform });
				return;
			}
		}

		// if we cant find the shader
		s_ObjectsToRender.push_back({ mesh, shader, transform });
		s_ShaderStartItorator.push_back(--s_ObjectsToRender.end());

	}

	void Renderer::SubmitLight(const PointLight& light)
	{
		if (s_Lights.numLights == MAX_LIGHTS) {
			DBOUT("max lights reached");
			return;
		}

		s_Lights.lights[s_Lights.numLights++] = light;
	}

	void Renderer::DrawMesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		glm::mat4 t;
		if (transform != glm::identity<glm::mat4>() || mesh->m_Transform != glm::identity<glm::mat4>())
			t = transform * mesh->m_Transform;

		// draw the sub meshes
		shader->SetBuffer("Model", (void*)&t);
		for (const auto& subMesh : mesh->m_Meshes)
		{
			subMesh->Bind();

			Ref<Material> material = subMesh->GetMaterial();
			if (material->m_Diffuse.get() != nullptr)
				material->m_Diffuse->Bind(0);
			else
				s_WhiteTexture->Bind(0);

			uint32_t count = subMesh->GetIndexBuffer()->GetCount();
			RendererCommand::DrawIndexed(count);
		}

		for (const auto& child : mesh->m_Children)
			DrawMesh(child, shader, transform);
	}




}