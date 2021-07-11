#include "Buffer.h"
#include "RendererAPI.h"

#include "Platform/DirectX11/DirectX11Buffer.h"

namespace Engine
{
	// ------------------------------------- Vertex Buffer ------------------------------------- //

#pragma region vertex buffer
	

	Ref<VertexBuffer> VertexBuffer::Create(const uint32_t size)
	{
		return Create(nullptr, size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, const uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return std::make_shared<DirectX11VertexBuffer>(vertices, size);
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}

#pragma endregion

	// ------------------------------------- End Vertex Buffer ------------------------------------- //

	// ------------------------------------- Index Buffer ------------------------------------- //

#pragma region index buffer

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t count)
	{
		return Create(nullptr, count);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return std::make_shared<DirectX11IndexBuffer>(indices, count);
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}

#pragma endregion

	// ------------------------------------- End Index Buffer ------------------------------------- //

	Ref<ConstentBuffer> ConstentBuffer::ConstentBuffer::Create(uint32_t size)
	{
		return Create(nullptr, size);
	}


	Ref<ConstentBuffer> ConstentBuffer::Create(const void* data, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return std::make_shared<DirectX11ConstentBuffer>(data, size);
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}


	Ref<StructuredBuffer> StructuredBuffer::Create(uint32_t elementSize, uint32_t elementCount)
	{
		switch (RendererAPI::GetAPI())
		{
		case Engine::RendererAPI::DirectX11:
			return std::make_shared<DirectX11StructuredBuffer>(elementSize, elementCount);
		case Engine::RendererAPI::OpenGL:
			break;
		case Engine::RendererAPI::Vulkan:
			break;
		default:
			break;
		}
	}

}