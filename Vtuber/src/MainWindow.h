#pragma once
#include "Core/Window.h"

#include "Renderer/Buffer.h"
#include "Renderer/Mesh.h"
#include "Renderer/RendererCommand.h"
#include "Renderer/Shader.h"

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
	Engine::Ref<Engine::VertexBuffer> vb;
	Engine::Ref<Engine::IndexBuffer> ib;
	Engine::Ref<Engine::Shader> shader;

	Engine::Mesh::Vertex* vertices;

	uint32_t* indices;
};