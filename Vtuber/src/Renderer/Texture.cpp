#include "Texture.h"
#include "RendererAPI.h"

#ifdef PLATFORM_WINDOWS
#include "Platform/DirectX11/DirectX11Texture.h"
#endif

namespace Engine 
{
	Ref<Texture2D> Texture2D::Create(const std::string& path, TextureAttribute& attribute)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::DirectX11:
			return std::make_shared<DirectX11Texture>(path, std::make_shared<TextureAttribute>(attribute));
		default:
			break;
		}
	}

	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, TextureAttribute& attribute)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::DirectX11:
			return std::make_shared<DirectX11Texture>(width, height, std::make_shared<TextureAttribute>(attribute));
		default:
			break;
		}
	}

}