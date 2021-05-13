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
	{}

protected:
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;

private:
	Engine::Ref<Engine::VertexBuffer> vb;
	Engine::Ref<Engine::IndexBuffer> ib;
	Engine::Ref<Engine::Shader> shader;
};