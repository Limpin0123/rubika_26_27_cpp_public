#include "Scene.h"
#include "TransformComponent.h"

Scene::Scene(const std::string& name)
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
}

Entity& Scene::AddEntity(std::string entityName)
{
	Entities.push_back(new Entity(entityName));
	Entities[Entities.size() - 1]->Start();

	return *Entities[Entities.size()-1];
}

void Scene::RemoveEntity(Entity* entityptr)
{
	for (int i = 0; i < Entities.size(); i++)
	{
		if (Entities[i] == entityptr) 
		{
			entityptr->Destroy();
			delete entityptr;
			Entities.erase(Entities.begin() + i);
		}
	}
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (Entity* entity : Entities)
	{
		entity->Draw(window);
	}
}

void Scene::DestroyAllEntities()
{
	for (Entity* entity : Entities)
	{
		RemoveEntity(entity);
	}
}

int Scene::GetEntityAmount()
{
	return Entities.size();
}
std::vector<Entity*>& Scene::GetEntities()
{
	return Entities;
}