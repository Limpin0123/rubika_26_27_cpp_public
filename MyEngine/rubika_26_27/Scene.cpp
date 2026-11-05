#include "Scene.h"
#include "TransformComponent.h"

Scene::Scene(const std::string& name) : staticColliders(sf::Vector2i(50,650), 15)
{
	this->name = name;
}

Scene::~Scene()
{
}

void Scene::Start()
{
	for (Entity* entity : Entities)
	{
		entity->Start();
	}
}
void Scene::Update(float fDeltaTime)
{
	for (Entity* entity : Entities)
	{
		entity->Update(fDeltaTime);
	}
}
void Scene::Destroy()
{
	DestroyAllEntities();
	dynamicColliders.clear();
	staticColliders.clear();
}

Entity& Scene::AddEntity(std::string entityName)
{
	Entities.push_back(new Entity(entityName));
	Entities[Entities.size() - 1]->Start();

	return *Entities[Entities.size() - 1];
}

void Scene::AddEntity(Entity* entity)
{
	Entities.push_back(entity);
	Entities[Entities.size() - 1]->Start();

	DynamicColliderComponent* collider = entity->GetComponent<DynamicColliderComponent>();
	if(collider == nullptr) return;
	if (entity->GetTag()->ContainsExactly(COLLIDER_TAG_DYNAMIC))
	{
		dynamicColliders.push_back(collider);
	}
	else
	{
		staticColliders.AddCollider(collider);
	}
}

void Scene::RemoveEntity(Entity* entityptr)
{
	for (auto i = Entities.begin(); i != Entities.end(); i++)
	{
		if (*i == entityptr)
		{
			DynamicColliderComponent* collider = entityptr->GetComponent<DynamicColliderComponent>();
			if (collider != nullptr)
			{
				if (!TryRemoveCollider(dynamicColliders, collider))
				{
					staticColliders.RemoveCollider(collider);
				}
			}

			entityptr->Destroy();
			delete entityptr;
			//erase return the next iterator in the container
			i = Entities.erase(i);
			break;
		}
	}
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (Entity* entity : Entities)
	{
		entity->Draw(window);
	}

#ifdef _DEBUG
	staticColliders.Draw(window);
#endif // _DEBUG
}

bool Scene::TryRemoveCollider(std::vector<DynamicColliderComponent*> container, DynamicColliderComponent* collider)
{
	for (auto i = container.begin(); i != container.end(); i++)
	{
		if (*i == collider) {
			container.erase(i);
			return true;
		}
	}
	return false;
}

void Scene::DestroyAllEntities()
{
	for (Entity* entity : Entities)
	{
		entity->Destroy();
		delete entity;
	}
	Entities.clear();
}

int Scene::GetEntityAmount()
{
	return Entities.size();
}
std::vector<Entity*>& Scene::GetEntities()
{
	return Entities;
}

std::vector<DynamicColliderComponent*>& Scene::GetDynamicColliders()
{
	return dynamicColliders;
}

void Scene::GetstaticCollidersAtPosition(std::vector<DynamicColliderComponent*>* colliders, DynamicColliderComponent* collider)
{
	return staticColliders.GetColliders(colliders, collider);
}
