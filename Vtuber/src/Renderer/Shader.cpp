#include "Shader.h"
#include "RendererAPI.h"

#include "Platform/DirectX11/DirectX11Shader.h"



namespace Engine
{

	Ref<Shader> Shader::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return std::make_shared<DirectX11Shader>();
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}

}