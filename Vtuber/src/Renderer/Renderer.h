#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Engine
{
	class Renderer
	{
	public:
		static void StartScene(const Ref<Camera>& camera);
		static void StartScene(const Ref<glm::mat4>& viewPorjectionMatrix);

		static void EndScene();

		static void Submit(const Ref<Mesh>& mesh, const Ref<Shader>& shader, const glm::mat4& transform);

	private:

		static glm::mat4 m_ViewProjectionMatrix;
	};
}