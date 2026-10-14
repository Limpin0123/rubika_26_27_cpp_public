#include "Entity.h"

#include "IComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

#include "Scene.h"


Entity::Entity(std::string friendlyName)
{
	name = friendlyName;
}

Entity::~Entity()
{
	
}

void Entity::Start()
{
	for (IComponent* component : Components)
	{
		component->Start();
	}
}
void Entity::Update(float fDeltaTime)
{
	for (IComponent* component : Components)
	{
		component->Update(fDeltaTime);
	}
}
void Entity::Destroy()
{
	for (IComponent* component : Components)
	{
		component->Destroy();
	}
}

void Entity::Draw(sf::RenderWindow& window) const
{
	for (IDrawableComponent* dc : DrawableComponents)
	{
		dc->Draw(window);
	}
	/*RenderComponent* rc = GetComponent<RenderComponent>();
	if (rc != nullptr)
	{
		rc->Draw(window);
	}*/
}
