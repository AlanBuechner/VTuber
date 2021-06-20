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
		//vertices = new Engine::Mesh::Vertex[]{
		//	// Position						Normal					    Color						UV
		//	{ {-0.5f, -0.5f, 0.5f, 1.0f }, { -0.577f, -0.577f, 0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
		//	{ {-0.5f,  0.5f, 0.5f, 1.0f }, { -0.577f,  0.577f, 0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
		//	{ { 0.5f,  0.5f, 0.5f, 1.0f }, {  0.577f,  0.577f ,0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
		//	{ { 0.5f, -0.5f, 0.5f, 1.0f }, {  0.577f, -0.577f, 0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

		//	{ {-0.5f, -0.5f, -0.5f, 1.0f }, { -0.577f, -0.577f, -0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
		//	{ {-0.5f,  0.5f, -0.5f, 1.0f }, { -0.577f,  0.577f, -0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
		//	{ { 0.5f,  0.5f, -0.5f, 1.0f }, {  0.577f,  0.577f ,-0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
		//	{ { 0.5f, -0.5f, -0.5f, 1.0f }, {  0.577f, -0.577f, -0.577f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
		//};

		//indices = new uint32_t[]{
		//	0, 1, 2,
		//	2, 3, 0,
		//	4, 6, 5,
		//	7, 6, 4,
		//	3, 2, 6,
		//	3, 6, 7,
		//	1, 5, 6,
		//	1, 6, 2,
		//	0, 5, 1,
		//	0, 4, 5,
		//	3, 4, 0,
		//	3, 7, 4,
		//};

		//m_Mesh = Engine::Mesh::Create(vertices, 8, indices, 3*2*6);
	}

	~MainWindow()
	{
		//delete[] vertices;
		//delete[] indices;
	}

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;

private:
	Engine::Ref<Engine::Mesh> m_Mesh;
	Engine::Ref<Engine::Shader> shader;
	Engine::Ref<Engine::Texture2D> m_Texture;

	/*Engine::Mesh::Vertex* vertices;
	uint32_t* indices;*/

	Engine::Ref<Engine::Camera> m_Camera;

	Engine::PointLight m_BlueLight;
	Engine::PointLight m_GreenLight;
};