#include "Mesh.h"

#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{

	// ------------------------- Sub Mesh ------------------------- //
#pragma region SubMesh
	glm::vec2 GetUVCoords(aiMesh* mesh, uint32_t index)
	{
		if (!mesh->HasTextureCoords(index) || mesh->mTextureCoords[0] == nullptr)
			return { 0.0f, 0.0f };
		const auto coords = mesh->mTextureCoords[0][index];
		return { coords.x, coords.y };
	}

	glm::vec4 GetColor(aiMesh* mesh, uint32_t index)
	{
		if (mesh->HasVertexColors(index) || mesh->mColors[0] == nullptr)
			return { 1.0f, 1.0f, 1.0f, 1.0f };
		const auto color = mesh->mColors[0][index];
		return { color.r, color.g, color.b, color.a };
	}


	Mesh::SubMesh::SubMesh(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount)
	{
		SetData(vertices, vertCount, indeces, indexCount);
	}

	void Mesh::SubMesh::Bind()
	{
		vb->Bind();
		ib->Bind();
	}

	void Mesh::SubMesh::UpdateVertexBuffer(Vertex* vertices, uint32_t count)
	{
		vb->SetData(vertices, count * sizeof(Vertex));
	}

	void Mesh::SubMesh::UpdateIndexBuffer(uint32_t* indeces, uint32_t count)
	{
		ib->SetData(indeces, count);
	}

	void Mesh::SubMesh::SetData(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount)
	{
		Engine::BufferLayout layout = {
			{Engine::ShaderDataType::Float4, "POSITION"},
			{Engine::ShaderDataType::Float3, "NORMAL"},
			{Engine::ShaderDataType::Float4, "COLOR"},
			{Engine::ShaderDataType::Float2, "TEXTCOORD"}
		};

		vb = VertexBuffer::Create(vertices, vertCount * sizeof(Vertex));
		vb->SetLayout(layout);

		ib = IndexBuffer::Create(indeces, indexCount);

		Bind();
	}

	Ref<Mesh::SubMesh> Mesh::SubMesh::Create(Vertex* vertices, uint32_t vertCount, uint32_t* indeces, uint32_t indexCount)
	{
		return std::make_shared<SubMesh>(vertices, vertCount, indeces, indexCount);
	}


#pragma endregion


	// ------------------------- Mesh ------------------------- //

	Mesh::Mesh(const std::string& filename)
	{
		LoadFromFile(filename);
	}

	void Mesh::LoadFromFile(const std::string& filename)
	{
		// load mesh data from the file
		Assimp::Importer imp;
		auto model = imp.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_FlipWindingOrder
		);


		// Generate all the meshes
		std::vector<Ref<SubMesh>> meshes;
		meshes.reserve(model->mNumMeshes);
		for (uint32_t i = 0; i < model->mNumMeshes; i++)
		{
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			const auto mesh = model->mMeshes[i]; // get the mesh

			// resize the verices array and load the data
			vertices.reserve(mesh->mNumVertices);
			for (uint32_t j = 0; j < mesh->mNumVertices; j++)
			{
				vertices.push_back({
					{ mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f },
					{ mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z },
					GetColor(mesh, j),
					GetUVCoords(mesh, j)
					});
			}

			// resize the index array and load the data
			indices.reserve(mesh->mNumFaces * 3);
			for (uint32_t j = 0; j < mesh->mNumFaces; j++)
			{
				const auto face = mesh->mFaces[j];
				for (uint32_t k = 0; k < 3; k++)
					indices.push_back(face.mIndices[k]);
			}

			Ref<SubMesh> subMesh = SubMesh::Create(vertices.data(), vertices.size(), indices.data(), indices.size());
			meshes.push_back(subMesh);
		}

		
		LoadNodeData(model->mRootNode, meshes);

	}

	Ref<Mesh> Mesh::Create(const std::string& filename)
	{
		return std::make_shared<Mesh>(filename);
	}

	void Mesh::LoadNodeData(aiNode* node, std::vector<Ref<SubMesh>> meshes)
	{
		// load meshes
		m_Meshes.reserve(node->mNumMeshes);
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
			m_Meshes.push_back(meshes[node->mMeshes[i]]);

		// load child nodes
		m_Children.reserve(node->mNumChildren);
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			Ref<Mesh> child = std::make_shared<Mesh>();
			child->LoadNodeData(node->mChildren[i], meshes);
			m_Children.push_back(child);
		}

		m_Transform = *reinterpret_cast<glm::mat4*>(&node->mTransformation);

	}

}