#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <vector>
#include <list>

#include "Renderer2D.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const Ref<Camera>& camera);
		static void BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		static void EndScene();

		static void SetAmbientLight(glm::vec3 light) { s_LightData.ambient = light; };

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

		static std::list<RenderObject> s_ObjectsToRender;
		static std::list<std::list<RenderObject>::const_iterator> s_ShaderStartItorator;

		static struct Lights {
			glm::vec3 ambient = { 0.15, 0.15, 0.15 };
		} s_LightData;
		static std::vector<PointLight> s_PointLights;

		static struct CameraData
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ViewProjectionMatrix;
		} s_Camera;

		static Ref<Texture2D> s_WhiteTexture;
		static Ref<Texture2D> s_BlackTexture;
	};
}