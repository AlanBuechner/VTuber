#include "Camera.h"

namespace Engine
{
	void Camera::SetAspect(float aspect)
	{
		m_AspectRatio = aspect;
		RecalculateProjection();
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void Camera::SetOrthgraphic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthgraphicSize = size;
		m_OrthgraphicNear = nearClip;
		m_OrthgraphicFar = farClip;

		RecalculateProjection();
	}

	void Camera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	Ref<Camera> Camera::Create(ProjectionType type, float fovSize, float nearClip, float farClip, float aspect)
	{
		Ref<Camera> cam = std::make_shared<Camera>();
		cam->m_AspectRatio = aspect;

		switch (type)
		{
		case Engine::Camera::ProjectionType::Perspective:
			cam->SetPerspective(fovSize, nearClip, farClip);
			break;
		case Engine::Camera::ProjectionType::Orthographic:
			cam->SetOrthgraphic(fovSize, nearClip, farClip);
			break;
		default:
			break;
		}

		return cam;
	}

	void Camera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthLeft = -0.5f * m_AspectRatio * m_OrthgraphicSize;
			float orthRight = 0.5f * m_AspectRatio * m_OrthgraphicSize;
			float orthBottom = -0.5f * m_OrthgraphicSize;
			float orthTop = 0.5f * m_OrthgraphicSize;

			m_ProjectionMatrix = glm::ortho(orthLeft, orthRight, orthBottom, orthTop, m_OrthgraphicNear, m_OrthgraphicFar);
		}
	}


}