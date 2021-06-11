#include "Mesh.h"

namespace Engine
{
	Mesh::Mesh(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount)
	{
		
		Engine::BufferLayout layout = {
			{Engine::ShaderDataType::Float4, "Position"}
		};

		vb = VertexBuffer::Create(vertices, vertCount * sizeof(Vertex));
		vb->SetLayout(layout);

		ib = IndexBuffer::Create(indeces, indexCount);

		Bind();
	}

	void Mesh::Bind()
	{
		vb->Bind();
		ib->Bind();
	}

	void Mesh::UpdateVertexBuffer(Vertex* vertices, uint32_t count)
	{
		vb->SetData(vertices, count * sizeof(Vertex));
	}

	void Mesh::UpdateIndexBuffer(uint32_t* indeces, uint32_t count)
	{
		ib->SetData(indeces, count);
	}

	Ref<Mesh> Mesh::Create(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount)
	{
		return std::make_shared<Mesh>(vertices, vertCount, indeces, indexCount);
	}

}