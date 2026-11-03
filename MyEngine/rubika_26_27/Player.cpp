#include "Player.h"
#include "TransformComponent.h"

Player::Player(std::string friendlyName) : Entity(friendlyName)//, movementInputs(0,0)
{
	movementInputs = sf::Vector2f(0,0);
	transform = AddComponent<TransformComponent>();
}

void Player::MoveUp(float value)
{
	movementInputs.y = -value * 0.5f;
}

void Player::MoveDown(float value)
{
	movementInputs.y = value * 0.5f;
}

void Player::MoveLeft(float value)
{
	movementInputs.x = -value * 0.5f;
}

void Player::MoveRight(float value)
{
	movementInputs.x = value * 0.5f;
}

void Player::Update(float fDeltaTime)
{
	Entity::Update(fDeltaTime);
	//Logger::Info("X:" + std::to_string(movementInputs.x) + " Y:" + std::to_string(movementInputs.y));
	transform->SetWorldPosition(transform->GetWorldPosition() + movementInputs);
}
