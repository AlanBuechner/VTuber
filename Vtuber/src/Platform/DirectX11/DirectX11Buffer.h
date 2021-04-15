#pragma once
#include "Platform/Windows/Win.h"
#include "Core/Core.h"
#include "Renderer/Buffer.h"

namespace Engine
{
	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer(const void* vertices, uint32_t size);
		virtual ~DirectX11VertexBuffer() override = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual BufferLayout& GetLayout() override { return m_Layout; }

		virtual void SetData(const void* data, const uint32_t size) override;

	private:
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		BufferLayout m_Layout;

	};

	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer(const uint32_t* indeces, const uint32_t count);
		virtual ~DirectX11IndexBuffer() override = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const uint32_t* indices, uint32_t size) override;

		virtual uint32_t GetCount() const override { return count; }

	private:
		uint32_t count;

		wrl::ComPtr<ID3D11Buffer> m_Buffer;
	};

}