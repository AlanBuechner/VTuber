#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <vector>
#include <list>

namespace Engine
{
	class Renderer
	{
	public:
		static void StartScene(const Ref<Camera>& camera);
		static void StartScene(const glm::mat4& viewPorjectionMatrix);

		static void EndScene();

		static void Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform);
		static void SubmitLight(const PointLight& light);

		static void DrawMesh(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform);

	private:
		struct RenderObject
		{
			const Ref<Mesh> mesh;
			const Ref<Shader> shader;
			glm::mat4 transform;
		};

		static std::list<RenderObject> m_ObjectsToRender;
		static std::list<std::list<RenderObject>::const_iterator> m_ShaderStartItorator;

		static struct Lights {
			uint32_t numLights;
			PointLight lights[MAX_LIGHTS];
		} m_Lights;
		static glm::mat4 m_ViewProjectionMatrix;
	};
}