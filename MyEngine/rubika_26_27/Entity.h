#pragma once
#include <string>
#include <functional>
#include "IDrawableComponent.h"
#include <SFML/Graphics.hpp>
#include "TagSystem.h"

class IComponent;
class TransformComponent;
class RenderComponent;

namespace sf
{
	class RenderWindow;
}

struct TagUpdateListener
{
	std::function<void(const TagSystem*)> callback;
	void* owner;

	TagUpdateListener(std::function<void(const TagSystem*)> clb, void* own)
	{
		callback = clb;
		owner = own;
	}
};

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	std::string name;

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	template <typename T>
	T* AddComponent();
	template <typename T>
	void RemoveComponent();
	template <typename T>
	T* GetComponent() const;
	template <typename T>
	bool CanAddComponent();

	const TagSystem* GetTag() const;
	void AddTag(TAG_TYPE tag);
	void RemoveTag(TAG_TYPE tag);
	void RegisterToTagUpdate(std::function<void(const TagSystem*)> callback, void* owner);

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
	void DrawDebug();

private:
	TagSystem Tags;

	std::vector<IComponent*> Components;
	std::vector<IDrawableComponent*> DrawableComponents;

	std::vector<TagUpdateListener> OnTagChangeCallback;
};

#include "Entity.hxx"