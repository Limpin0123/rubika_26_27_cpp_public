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

void Scene::AddEntity(Entity* entity)
{
	Entities.push_back(entity);
	Entities[Entities.size() - 1]->Start();
}

void Scene::RemoveEntity(Entity* entityptr)
{
	for (auto i = Entities.begin(); i != Entities.end(); i++)
	{
		if (*i == entityptr)
		{
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