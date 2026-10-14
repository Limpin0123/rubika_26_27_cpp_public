#include "Entity.h"

#include "Logger.h"
#include "Scene.h"


Entity::Entity(std::string friendlyName) : Transform(*this), Render(*this)
{
	name = friendlyName;
}

Entity::~Entity()
{
	
}

void Entity::Start()
{
	Render.Start();
	Transform.Start();
}
void Entity::Update(float fDeltaTime)
{
	Render.Update(fDeltaTime);
	Transform.Update(fDeltaTime);
}
void Entity::Destroy()
{
	Render.Destroy();
	Transform.Destroy();
}

void Entity::Draw(sf::RenderWindow& window) const
{
	Render.Draw(window);
}
