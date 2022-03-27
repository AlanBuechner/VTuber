#pragma once
#include "Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
	class Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	protected:
		typedef Engine::Camera Super;

	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix) :
			m_ProjectionMatrix(projectionMatrix)
		{}

		virtual ~Camera() = default;

		void SetAspect(float aspect);
		void SetViewportSize(uint32_t width, uint32_t height);

		void SetOrthgraphic(float size, float nearClip, float farClip);
		float GetOrthographicSize() { return m_OrthgraphicSize; }
		void SetOrthographicSize(float size) { m_OrthgraphicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() { return m_OrthgraphicNear; }
		void SetOrthographicNearClip(float nearPlane) { m_OrthgraphicNear = nearPlane; RecalculateProjection(); }
		float GetOrthographicFarClip() { return m_OrthgraphicFar; }
		void SetOrthographicFarClip(float farPlane) { m_OrthgraphicFar = farPlane; RecalculateProjection(); }

		void SetPerspective(float fov, float nearClip, float farClip);
		float GetPerspectiveVerticalFOV() { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		float GetPerspectiveNearClip() { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearPlane) { m_PerspectiveNear = nearPlane; RecalculateProjection(); }
		float GetPerspectiveFarClip() { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farPlane) { m_PerspectiveFar = farPlane; RecalculateProjection(); }

		ProjectionType GetType() const { return m_ProjectionType; }
		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() { return glm::mat4(1.0f); }

		static Ref<Camera> Create(ProjectionType type, float fovSize, float nearClip, float farClip, float aspect);

	private:
		void RecalculateProjection();

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthgraphicSize = 10.0f;
		float m_OrthgraphicNear = -1.0f;
		float m_OrthgraphicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.001f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 0.0f;
	};
}