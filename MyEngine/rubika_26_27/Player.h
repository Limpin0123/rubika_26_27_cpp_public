#pragma once

#include "SFML/Graphics/Transform.hpp"
#include "Entity.h"

class Player : public Entity
{
public:
	Player(std::string friendlyName = "");

	void MoveUp(float value);
	void MoveDown(float value);
	void MoveLeft(float value);
	void MoveRight(float value);

	virtual void Update(float fDeltaTime) override;

private:
	TransformComponent* transform;
	sf::Vector2f movementInputs;
};

