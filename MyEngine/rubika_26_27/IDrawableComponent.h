#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class IDrawableComponent abstract
{
public:
	virtual void Draw(sf::RenderWindow& window) const = 0;
};

