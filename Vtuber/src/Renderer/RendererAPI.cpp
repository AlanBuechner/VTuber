#include "RendererAPI.h"

#include "Platform/DirectX11/DirectX11RendererAPI.h"

Engine::RendererAPI* Engine::RendererAPI::s_Instance;
Engine::RendererAPI::API Engine::RendererAPI::s_API;

namespace Engine
{
	void RendererAPI::CreateAPI(API api)
	{
		s_API = api;
		switch (api)
		{
		case Engine::RendererAPI::DirectX11:
			s_Instance = new DirectX11RendererAPI();
			break;
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}
}