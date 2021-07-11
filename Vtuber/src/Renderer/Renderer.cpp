#include "Renderer/Renderer.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"

Engine::Renderer::Lights Engine::Renderer::s_LightData;
std::vector<Engine::PointLight> Engine::Renderer::s_PointLights;
Engine::Renderer::CameraData Engine::Renderer::s_Camera;
std::list<Engine::Renderer::RenderObject> Engine::Renderer::s_ObjectsToRender;
std::list<std::list<Engine::Renderer::RenderObject>::const_iterator> Engine::Renderer::s_ShaderStartItorator;
Engine::Ref<Engine::Texture2D> Engine::Renderer::s_WhiteTexture;
Engine::Ref<Engine::Texture2D> Engine::Renderer::s_BlackTexture;

namespace Engine
{
	void Renderer::Init()
	{
		s_WhiteTexture = Texture2D::Create(1,1);
		unsigned char blackTextureData[]{ 0x00 };
		s_BlackTexture = Texture2D::Create(1,1, blackTextureData);

		Renderer2D::Init();
	}

	void Renderer::BeginScene(const Ref<Camera>& camera)
	{
		BeginScene(glm::mat4(1.0f), camera->GetProjectionMatrix());
	}

	void Renderer::BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		s_Camera.ViewMatrix = glm::inverse(viewMatrix);
		s_Camera.ViewProjectionMatrix = projectionMatrix * s_Camera.ViewMatrix;
		s_PointLights.clear();
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
				o.shader->SetBuffer("Camera", (void*)&s_Camera);
				o.shader->SetBuffer("Lights", (void*)&s_LightData);
				o.shader->SetBuffer("PointLights", (void*)s_PointLights.data(), s_PointLights.size());
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
		s_PointLights.push_back(light); // add the new light
		PointLight& l = s_PointLights[s_PointLights.size()-1]; // get the light from the array
		l.position = s_Camera.ViewMatrix * glm::vec4(light.position, 1.0f); // change its position to be relitive to the camera
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
			if (material->m_Diffuse)
				material->m_Diffuse->Bind(0);
			else
				s_WhiteTexture->Bind(0);

			if (material->m_Specular)
				material->m_Specular->Bind(1);
			else
				s_BlackTexture->Bind(1);

			uint32_t count = subMesh->GetIndexBuffer()->GetCount();
			RendererCommand::DrawIndexed(count);
		}

		for (const auto& child : mesh->m_Children)
			DrawMesh(child, shader, transform);
	}




}