#pragma once

#include <string>
#include <vector>
#include "Entity.h"

namespace sf
{
	class RenderWindow;
}

class Scene
{
public:
	Scene(const std::string& name);
	~Scene();

	std::string name;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	virtual Entity& AddEntity(std::string entityName);
	virtual void RemoveEntity(Entity* entityptr);

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	int GetEntityAmount();
	std::vector<Entity>& GetEntities();

private:
	std::vector<Entity> Entities;
	
	void DestroyAllEntities();
};