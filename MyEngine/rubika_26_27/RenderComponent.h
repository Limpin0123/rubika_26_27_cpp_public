#pragma once

#include "IComponent.h"
#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable;

class RenderComponent : public IComponent
{

public:

	RenderComponent() = delete;
	RenderComponent(Entity& entity);
	~RenderComponent();

	void Draw(sf::RenderWindow& window) const;

	void Start() override;
	void Update(float fDelatTime) override;
	void Destroy() override;

private:
	IDrawable* Visual;
};

