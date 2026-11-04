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
	std::vector<Entity*> entities = SceneManager.activeScene->GetEntities();
	for (int i = 0; i < entities.size() - 1; i++)
	{
		DynamicColliderComponent* dcc = entities.at(i)->GetComponent<DynamicColliderComponent>();
		if (dcc == nullptr) continue;

		for (int i2 = i + 1; i2 < entities.size(); i2++)
		{
			DynamicColliderComponent* otherDcc = entities.at(i2)->GetComponent<DynamicColliderComponent>();
			if (otherDcc == nullptr) continue;

			
			std::optional<sf::FloatRect> collisionRect = dcc->GetHitbox().findIntersection(otherDcc->GetHitbox());

			if (collisionRect)
			{
				CollisionFilterMode myFilterMode = dcc->GetCollisionFilterMode();
				TAG_TYPE myFilterMask = dcc->GetCollisionFilterMask();

				CollisionFilterMode otherFilterMode = dcc->GetCollisionFilterMode();
				TAG_TYPE otherFilterMask = dcc->GetCollisionFilterMask();


				if (myFilterMode == CollisionFilterMode::ExactTags && !otherDcc->collisionTag.ContainsExactly(myFilterMask)) continue;
				else if (myFilterMode == CollisionFilterMode::AtLeastOneTag && !otherDcc->collisionTag.ContainsAtLeastOne(myFilterMask)) continue;
				dcc->OnCollision(*collisionRect, otherDcc);

				if (myFilterMode == CollisionFilterMode::ExactTags && !otherDcc->collisionTag.ContainsExactly(myFilterMask)) continue;
				else if (myFilterMode == CollisionFilterMode::AtLeastOneTag && !otherDcc->collisionTag.ContainsAtLeastOne(myFilterMask)) continue;
				otherDcc->OnCollision(*collisionRect, dcc);
			}
		}
	}
}
void PhysicMgr::Shut()
{}