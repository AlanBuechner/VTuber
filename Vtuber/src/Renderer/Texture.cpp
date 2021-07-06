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
		unsigned char* data = new unsigned char[width*height*4*sizeof(uint8_t)];
		for (uint32_t i = 0; i < width * height * sizeof(uint8_t) * 4; i++)
			data[i] = 0xff;

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::DirectX11:
			return std::make_shared<DirectX11Texture>(width, height, data, std::make_shared<TextureAttribute>(attribute));
		default:
			break;
		}

		delete[] data;
	}

	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height, unsigned char* data, TextureAttribute& attribute)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::DirectX11:
			return std::make_shared<DirectX11Texture>(width, height, data, std::make_shared<TextureAttribute>(attribute));
		default:
			break;
		}
	}

}