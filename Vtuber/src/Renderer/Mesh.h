#pragma once
#include "Core/Core.h"
#include <glm/glm.hpp>
#include <vector>

#include "Buffer.h"

namespace Engine
{
	class Mesh
	{
	public:
		struct Vertex {
			glm::vec4 Position;
			glm::vec4 Color;
			glm::vec2 TextCord;
		};

		Mesh(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount);

		void Bind();

		void UpdateVertexBuffer(Vertex* vertices, uint32_t count);
		void UpdateIndexBuffer(uint32_t* indeces, uint32_t count);

		Ref<VertexBuffer> GetVertexBuffer() { return vb; };
		Ref<IndexBuffer> GetIndexBuffer() { return ib; };

		static Ref<Mesh> Create(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount);

	private:
		Ref<VertexBuffer> vb;
		Ref<IndexBuffer> ib;
	};
}