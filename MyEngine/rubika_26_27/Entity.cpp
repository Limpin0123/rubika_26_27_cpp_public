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

const TagSystem* Entity::GetTag() const
{
	return &Tags;
}
void Entity::AddTag(TAG_TYPE tag) {
	Tags.AddTag(tag);
	for (TagUpdateListener listener : OnTagChangeCallback)
	{
		listener.callback(&Tags);
	}
}
void Entity::RemoveTag(TAG_TYPE tag) {
	Tags.RemoveTag(tag);
	for (TagUpdateListener listener : OnTagChangeCallback)
	{
		listener.callback(&Tags);
	}
}

void Entity::RegisterToTagUpdate(std::function<void(const TagSystem*)> callback, void* owner)
{
	TagUpdateListener listener = TagUpdateListener(callback, owner);
}

void Entity::Draw(sf::RenderWindow& window) const
{
	for (IDrawableComponent* dc : DrawableComponents)
	{
		dc->Draw(window);
	}
}

void Entity::DrawDebug()
{
	for (IComponent* component : Components)
	{
		component->DrawDebug();
	}
}
