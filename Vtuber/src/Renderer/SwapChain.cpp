#include "SwapChain.h"
#include "RendererAPI.h"

#include "Platform/DirectX11/DirectX11SwapChain.h"

namespace Engine
{
	SwapChain& SwapChain::CreateSwapChain()
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return *new DirectX11SwapChain();
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