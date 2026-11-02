#include "Player.h"
#include "TransformComponent.h"
#include "SFML/Graphics/Transform.hpp"

Player::Player(std::string friendlyName) : Entity(friendlyName)
{
	transform = AddComponent<TransformComponent>();
}

void Player::MoveUp(float)
{
	transform->SetWorldPosition(transform->GetWorldPosition() + sf::Vector2f(0, -1));
}

void Player::MoveDown(float)
{
	transform->SetWorldPosition(transform->GetWorldPosition() + sf::Vector2f(0, 1));
}

void Player::MoveLeft(float)
{
	transform->SetWorldPosition(transform->GetWorldPosition() + sf::Vector2f(-1,0));
}

void Player::MoveRight(float)
{
	transform->SetWorldPosition(transform->GetWorldPosition() + sf::Vector2f(1,0));
}
