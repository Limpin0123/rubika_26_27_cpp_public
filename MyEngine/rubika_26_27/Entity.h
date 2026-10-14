#pragma once
#include <string>
#include "IDrawableComponent.h"
#include <SFML/Graphics.hpp>

class IComponent;
class TransformComponent;
class RenderComponent;

namespace sf
{
	class RenderWindow;
}

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
	T* GetComponent() const;

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

private:
	std::vector<IComponent*> Components;
	std::vector<IDrawableComponent*> DrawableComponents;
};

#include "Entity.hxx"