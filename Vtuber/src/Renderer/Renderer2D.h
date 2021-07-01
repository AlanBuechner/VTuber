#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Light.h"
#include <glm/glm.hpp>

namespace Engine
{
	class Renderer2D
	{
	public:
		static void Init();

		static void SetShader(Ref<Shader>& shader) { m_Shader = shader; }
		static void BeginScene(const Ref<Camera>& camera);
		static void BeginScene(const glm::mat4& viewPorjectionMatrix);
		static void EndScene();

		static void DrawQuad(glm::mat4& transform, glm::vec4 color, Ref<Texture2D>& texture);

	private:
		static Ref<Shader> m_Shader;

	};
}