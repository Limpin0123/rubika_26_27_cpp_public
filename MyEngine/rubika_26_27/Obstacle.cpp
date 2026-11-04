#include "Obstacle.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "StaticDrawable.h"
#include "DynamicColliderComponent.h"

Obstacle::Obstacle(sf::Vector2f position, std::string friendlyName) : Entity(friendlyName)
{
	TransformComponent* tr = AddComponent<TransformComponent>();
	tr->SetWorldPosition(position);

	RenderComponent* rc = AddComponent<RenderComponent>();
	StaticDrawable* sd = rc->AddDrawable<StaticDrawable>();
	sd->SetTextureAndStaticTile("Basement.png", "Ground_Line_Up");

	DynamicColliderComponent* dcc = AddComponent<DynamicColliderComponent>();
	dcc->SetHitboxOffset(sf::Vector2f(10, 10));
	dcc->SetHitboxSize(sf::Vector2f(80, 80));

	std::function<OnCollisionChoice(DynamicColliderComponent*)> collisionCallback =
		[](DynamicColliderComponent* otherDcc) -> OnCollisionChoice
		{
			return OnCollisionChoice::Ignore;
		};
	dcc->SetOnCollisionCallback(collisionCallback);

	dcc->collisionTag.AddTag(COLLIDER_TAG_STATIC);
	dcc->SetCollisionMode(CollisionFilterMode::ExactTags);
	dcc->SetCollisionFilterMask(COLLIDER_TAG_DYNAMIC);
}
