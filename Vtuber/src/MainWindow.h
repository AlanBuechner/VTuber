#pragma once
#include "Core/Window.h"

#include "Renderer/Buffer.h"
#include "Renderer/Mesh.h"
#include "Renderer/RendererCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/Light.h"


class MainWindow : public Engine::Window
{
public:
	MainWindow(WindowProps props) :
		Window::Window(props)
	{

	}

	~MainWindow()
	{

	}

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;

private:
	Engine::Ref<Engine::Mesh> m_Mesh;
	Engine::Ref<Engine::Shader> shader;

	Engine::Ref<Engine::Camera> m_Camera;

	Engine::PointLight m_BlueLight;
	Engine::PointLight m_GreenLight;
	Engine::PointLight m_WhiteLight;
};