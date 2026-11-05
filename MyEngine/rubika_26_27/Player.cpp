#include "Player.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "AnimatedDrawable.h"
#include "DynamicColliderComponent.h"

Player::Player(std::string friendlyName) : Entity(strcmp(friendlyName.c_str(), "") == 0 ? "Player" : friendlyName)//, movementInputs(0,0)
{
	movementInputs = sf::Vector2f(0,0);
	transform = AddComponent<TransformComponent>();

	RenderComponent* rc = AddComponent<RenderComponent>();
	AnimatedDrawable* ad = rc->AddDrawable<AnimatedDrawable>();
	ad->SetTextureAndAnimation("IsaacSprite.png", "Body_Left");

	DynamicColliderComponent* dcc = AddComponent<DynamicColliderComponent>();
	dcc->SetHitboxOffset(sf::Vector2f(10, 10));
	dcc->SetHitboxSize(sf::Vector2f(80, 80));

	std::function<OnCollisionChoice(DynamicColliderComponent*)> collisionCallback =
		[](DynamicColliderComponent* otherDcc) -> OnCollisionChoice
		{
			return OnCollisionChoice::PushBack;
		};
	dcc->SetOnCollisionCallback(collisionCallback);

	AddTag(COLLIDER_TAG_DYNAMIC);
	//dcc->SetCollisionMode(CollisionFilterMode::AtLeastOneTag);
	//dcc->SetCollisionFilterMask(COLLIDER_TAG_DYNAMIC);
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
