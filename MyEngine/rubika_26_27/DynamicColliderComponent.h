#pragma once

#include <IComponent.h>
#include <TransformComponent.h>
#include <IDrawableComponent.h>

#include <SfML/Graphics.hpp>
#include <functional>
#include "TagSystem.h"

enum class OnCollisionChoice
{
	Ignore,
	PushBack,
};
enum class CollisionFilterMode
{
	ExactTags,
	AtLeastOneTag
};

class DynamicColliderComponent : public IComponent
#ifdef _DEBUG
	, public IDrawableComponent
#endif // _DEBUG

{
public:
	DynamicColliderComponent(Entity& entity);
	~DynamicColliderComponent();

	TagSystem collisionTag;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;
	virtual void DrawDebug() override;
#ifdef _DEBUG
	virtual void Draw(sf::RenderWindow& window) const override;
#endif // \DEBUG

	void OnCollision(sf::FloatRect collisionRect, DynamicColliderComponent* otherDcc);


	void SetHitboxSize(sf::Vector2f size);
	void SetHitboxOffset(sf::Vector2f offset);
	const sf::FloatRect GetHitbox() const;

	void SetCollisionFilterMask(TAG_TYPE tag);
	const TAG_TYPE GetCollisionFilterMask();
	void SetCollisionMode(CollisionFilterMode mode);
	const CollisionFilterMode GetCollisionFilterMode();

	void SetOnCollisionCallback(std::function<OnCollisionChoice(DynamicColliderComponent*)> callback);

private:
	sf::FloatRect Hitbox;
	sf::Vector2f HitboxSize;
	sf::Vector2f HitboxOffset;
	TransformComponent& transform;

	std::function<OnCollisionChoice(DynamicColliderComponent*)> CollisionCallback;

	TAG_TYPE CollisionFilter = COLLIDER_TAG_NONE;
	CollisionFilterMode collisionFilterMode = CollisionFilterMode::ExactTags;

#ifdef _DEBUG
	inline static bool showDebug = true;
	sf::RectangleShape debugShape;
#endif // _DEBUG
};