#pragma once

#include <entt.hpp>

namespace Engine 
{
	class Entity;
}

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdateRuntime();
		void OnViewPortResize(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = "");
		void DestroyEntity(Entity entity);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		friend Entity;

	};

}