#include "Entity.h"

#include "Logger.h"
#include "Scene.h"


Entity::Entity(std::string friendlyName)
{
	name = friendlyName;
	Transform = TransformComponent();
}

Entity::~Entity()
{
	
}

void Entity::Start()
{
	Logger::Info("Entity: " + name + " Started !");
}
void Entity::Update(float fDeltaTime)
{
	
}
void Entity::Destroy()
{

}

void Entity::Draw(sf::RenderWindow& window) const
{
	Logger::Info("Entity is Being Drawn");
}
