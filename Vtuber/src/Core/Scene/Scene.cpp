#include "Scene.h"
#include "Entity.h"

namespace Engine
{
	Scene::Scene()
	{ }

	Scene::~Scene()
	{ }

	void Scene::OnUpdateRuntime()
	{ 
		
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

}