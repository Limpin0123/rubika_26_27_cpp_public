#include "PhysicMgr.h"

#include <SfML/Graphics.hpp>

#include "SceneMgr.h"
#include "Scene.h"
#include "Entity.h"
#include "DynamicColliderComponent.h"


PhysicMgr::PhysicMgr(SceneMgr* sceneMgr) : SceneManager(*sceneMgr)
{}

void PhysicMgr::Init()
{ }
void PhysicMgr::Update(float fDeltaTime)
{
	std::vector<DynamicColliderComponent*> dynamicColliders = SceneManager.activeScene->GetDynamicColliders();

	for (auto i = dynamicColliders.begin(); i != dynamicColliders.end(); i++)
	{
		DynamicColliderComponent* collider = *i;
		CollisionFilterMode filterMode = collider->GetCollisionFilterMode();
		TAG_TYPE filterMask = collider->GetCollisionFilterMask();
		const TagSystem* tag = collider->GetEntity().GetTag();

		for (auto j = dynamicColliders.begin(); j != dynamicColliders.end(); j++)
		{
			DynamicColliderComponent* otherCollider = *j;
			
			if (collider == otherCollider) continue;

			std::optional<sf::FloatRect> collisionRect = collider->GetHitbox().findIntersection(otherCollider->GetHitbox());
			if (collisionRect)
			{
				const TagSystem* otherTag = otherCollider->GetEntity().GetTag();

				if ((filterMode == CollisionFilterMode::ExactTags && !otherTag->ContainsExactly(filterMask)) ||
					(filterMode == CollisionFilterMode::AtLeastOneTag && !otherTag->ContainsAtLeastOne(filterMask))
					) {
					continue;
				}
				
				collider->OnCollision(*collisionRect, otherCollider);
				otherCollider->OnCollision(*collisionRect, collider);
			}

		}

		std::vector<DynamicColliderComponent*> staticColliders;
		SceneManager.activeScene->GetstaticCollidersAtPosition(&staticColliders, collider);

		for (auto j = staticColliders.begin(); j != staticColliders.end(); j++)
		{
			DynamicColliderComponent* otherCollider = *j;

			std::optional<sf::FloatRect> collisionRect = collider->GetHitbox().findIntersection(otherCollider->GetHitbox());
			if (collisionRect)
			{
				const TagSystem* otherTag = otherCollider->GetEntity().GetTag();

				if ((filterMode == CollisionFilterMode::ExactTags && !otherTag->ContainsExactly(filterMask)) ||
					(filterMode == CollisionFilterMode::AtLeastOneTag && !otherTag->ContainsAtLeastOne(filterMask))
					) {
					continue;
				}

				collider->OnCollision(*collisionRect, otherCollider);
				otherCollider->OnCollision(*collisionRect, collider);
			}
		}
	}
}
void PhysicMgr::Shut()
{}