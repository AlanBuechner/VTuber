#pragma once
#include "Core/Window.h"

#include "Renderer/Buffer.h"
#include "Renderer/Mesh.h"
#include "Renderer/RendererCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

class MainWindow : public Engine::Window
{
public:
	MainWindow(WindowProps props) :
		Window::Window(props)
	{
		vertices = new Engine::Mesh::Vertex[]{
			{ { 0.0f,  0.5f, 0.0f, 1.0f} },
			{ { 0.5f, -0.5f, 0.0f, 1.0f} },
			{ {-0.5f, -0.5f, 0.0f, 1.0f} }
		};

		indices = new uint32_t[]{
			0, 1, 2
		};

		m_Mesh = Engine::Mesh::Create(nullptr, 3, indices, 3);
	}

	~MainWindow()
	{
		delete[] vertices;
		delete[] indices;
	}

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;

private:
	Engine::Ref<Engine::Mesh> m_Mesh;
	Engine::Ref<Engine::Shader> shader;

	Engine::Mesh::Vertex* vertices;

	uint32_t* indices;

	Engine::Ref<Engine::Camera> m_Camera;
};