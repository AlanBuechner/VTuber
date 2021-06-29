#pragma once
#include "Core/Core.h"
#include "Buffer.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>


namespace Engine
{

	class Mesh
	{
	public:
		struct Vertex {
			glm::vec4 Position;
			glm::vec3 Normal;
			glm::vec4 Color;
			glm::vec2 TextCord;
		};

		class SubMesh
		{
		public:


			SubMesh(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount);

			void Bind();

			void UpdateVertexBuffer(Vertex* vertices, uint32_t count);
			void UpdateIndexBuffer(uint32_t* indeces, uint32_t count);

			void SetData(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount);

			Ref<VertexBuffer> GetVertexBuffer() { return vb; };
			Ref<IndexBuffer> GetIndexBuffer() { return ib; };

			void SetMaterial(Ref<Material>& mat) { m_Material = mat; }
			Ref<Material> GetMaterial() { return m_Material; }

			static Ref<SubMesh> Create(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount);

		private:
			Ref<VertexBuffer> vb;
			Ref<IndexBuffer> ib;

			Ref<Material> m_Material;

		};


	public:
		Mesh() = default;
		Mesh(const std::string& filename);
		static Ref<Mesh> Create(const std::string& filename);

		void LoadFromFile(const std::string& filename);

	private:
		void LoadNodeData(aiNode* node, std::vector<Ref<SubMesh>>& meshes);

	public:

		std::vector<Ref<SubMesh>> m_Meshes;
		std::vector<Ref<Mesh>> m_Children;

		glm::mat4 m_Transform;
		
		static const std::string& s_TexturesFolder;
	};
}