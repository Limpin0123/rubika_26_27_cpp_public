#pragma once

#include "Entity.h"

class Obstacle : public Entity
{
public:
	Obstacle(sf::Vector2f position, std::string friendlyName = "");
};

