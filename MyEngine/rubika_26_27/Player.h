#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player(std::string friendlyName = "");

	void MoveUp(float);
	void MoveDown(float);
	void MoveLeft(float);
	void MoveRight(float);

private:
	TransformComponent* transform;
};

