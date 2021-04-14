#pragma once
#include "Platform/Windows/Win.h"
#include "Core/Core.h"

namespace Engine
{

#pragma region buffer layout

	enum class ShaderDataType
	{
		None,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:		return 4;
		case Engine::ShaderDataType::Float2:	return 4 * 2;
		case Engine::ShaderDataType::Float3:	return 4 * 3;
		case Engine::ShaderDataType::Float4:	return 4 * 4;
		case Engine::ShaderDataType::Mat3:		return 4 * 3 * 3;
		case Engine::ShaderDataType::Mat4:		return 4 * 4 * 4;
		case Engine::ShaderDataType::Int:		return 4;
		case Engine::ShaderDataType::Int2:		return 4 * 2;
		case Engine::ShaderDataType::Int3:		return 4 * 3;
		case Engine::ShaderDataType::Int4:		return 4 * 4;
		case Engine::ShaderDataType::Bool:		return 1;
		}
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}

		uint32_t GetComponentCount()
		{
			switch (Type)
			{
			case Engine::ShaderDataType::Float:		return 1;
			case Engine::ShaderDataType::Float2:	return 2;
			case Engine::ShaderDataType::Float3:	return 3;
			case Engine::ShaderDataType::Float4:	return 4;
			case Engine::ShaderDataType::Mat3:		return 3 * 3;
			case Engine::ShaderDataType::Mat4:		return 4 * 4;
			case Engine::ShaderDataType::Int:		return 1;
			case Engine::ShaderDataType::Int2:		return 2;
			case Engine::ShaderDataType::Int3:		return 3;
			case Engine::ShaderDataType::Int4:		return 4;
			case Engine::ShaderDataType::Bool:		return 1;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
			}
			m_Stride = offset;
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

#pragma endregion

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* vertices, const uint32_t size);
		~VertexBuffer() = default;

		void Bind() const;
		void Unbind() const;

		void SetLayout(const BufferLayout& layout);
		inline BufferLayout& GetLayout() { return m_Layout; }

		void SetData(const void* data, const uint32_t size);

		static Ref<VertexBuffer> Create(const uint32_t size);
		static Ref<VertexBuffer> Create(const void* vertices, uint32_t size);

	private:
		BufferLayout m_Layout;
		wrl::ComPtr<ID3D11Buffer> m_Buffer;

	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const uint32_t* indeces, const uint32_t count);
		~IndexBuffer() = default;

		void Bind() const;
		void Unbind() const;

		void SetData(const uint32_t* indices, uint32_t size);

		inline uint32_t GetCount() const { return count; };
		
		static Ref<IndexBuffer> Create(const uint32_t count);
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);

	private:
		uint32_t count;

		wrl::ComPtr<ID3D11Buffer> m_Buffer;
	};
}
