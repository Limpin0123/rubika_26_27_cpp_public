#include "DynamicColliderComponent.h"
#include <Entity.h>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <functional>
#include "Globals.h"
#endif // _USE_IMGUI


DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : 
	IComponent(entity), 
	Hitbox({0,0},{1,1}), 
	transform(*entity.GetComponent<TransformComponent>())
{
	HitboxSize = sf::Vector2f(1, 1);
	HitboxOffset = sf::Vector2f(0, 0);
#ifdef _DEBUG
	debugShape.setSize(sf::Vector2f(1,1));
	debugShape.setPosition(transform.GetWorldPosition());
	debugShape.setFillColor(sf::Color(0, 0, 0, 0));
	debugShape.setOutlineColor(sf::Color(0,255,0,255));
	debugShape.setOutlineThickness(5.0f);

	std::function<void(void*)> debugAction = [](void* v) -> void
		{
			DynamicColliderComponent::showDebug = !DynamicColliderComponent::showDebug;
		};
	gData.DebugMgr->RegisterDebugAction("Collider Visibility", debugAction, this);
#endif // DEBUG
}

DynamicColliderComponent::~DynamicColliderComponent()
{}

void DynamicColliderComponent::Start()
{
}

void DynamicColliderComponent::Update(float fDeltaTime)
{
	Hitbox.position = transform.GetWorldPosition() + HitboxOffset;
	Hitbox.size = sf::Vector2f(transform.GetScale().x * HitboxSize.x, transform.GetScale().y * HitboxSize.y);
#ifdef _DEBUG
	debugShape.setPosition(Hitbox.position);
	debugShape.setSize(Hitbox.size);
#endif // _DEBUG
}

void DynamicColliderComponent::Destroy()
{
}

void DynamicColliderComponent::DrawDebug()
{
	ImGui::PushID(this);
	int flags = ImGuiInputTextFlags_CharsDecimal;
	ImGui::TextColored(ImVec4(255, 0, 0, 255), "Transform Component");
	if (ImGui::InputFloat2("Size", &HitboxSize.x, "%.3f", flags))
	{
		SetHitboxSize(HitboxSize);
	}
	if (ImGui::InputFloat2("Offset", &HitboxOffset.x, "%.3f", flags))
	{
		SetHitboxOffset(HitboxOffset);
	}
#ifdef _DEBUG
	if (ImGui::Checkbox("Show Debug", &showDebug))
	{

	}
#endif // _DEBUG
	ImGui::PopID();
}

#ifdef _DEBUG
void DynamicColliderComponent::Draw(sf::RenderWindow& window) const
{
	if (!showDebug) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Vector2f transformPosition = transform.GetWorldPosition();
	states.transform.transformPoint(Hitbox.position);

	window.draw(debugShape, states);
}
#endif // _DEBUG

void DynamicColliderComponent::OnCollision(sf::FloatRect collisionRect, DynamicColliderComponent* otherDcc)
{
	//TODO Manage Collision
	//Logger::Info("Collision: " + GetEntity().name + " with " + otherDcc->GetEntity().name);

	if (!CollisionCallback) return;
	OnCollisionChoice reactionType = CollisionCallback(otherDcc);

	if (reactionType == OnCollisionChoice::Ignore) return;

	else if (reactionType == OnCollisionChoice::PushBack)
	{
		if (collisionRect.size.x < collisionRect.size.y)
		{
			float moveX = transform.GetWorldPosition().x > otherDcc->GetHitbox().position.x ?
				collisionRect.size.x * 0.5f :
				-collisionRect.size.x * 0.5f;
			transform.SetWorldPosition(transform.GetWorldPosition() + sf::Vector2f(moveX, 0));
		}
		else
		{
			float moveY = transform.GetWorldPosition().y > otherDcc->GetHitbox().position.y ?
				collisionRect.size.y * 0.5f :
				-collisionRect.size.y * 0.5f;
			transform.SetWorldPosition(transform.GetWorldPosition() + sf::Vector2f(0, moveY));
		}
	}
}

void DynamicColliderComponent::SetHitboxSize(sf::Vector2f size)
{
	HitboxSize = size;
	Hitbox.size = sf::Vector2f(transform.GetScale().x * HitboxSize.x, transform.GetScale().y * HitboxSize.y);
}

void DynamicColliderComponent::SetHitboxOffset(sf::Vector2f offset)
{
	HitboxOffset = offset;
	Hitbox.position = transform.GetWorldPosition() + HitboxOffset;
}

const sf::FloatRect DynamicColliderComponent::GetHitbox() const
{
	return Hitbox;
}

void DynamicColliderComponent::SetCollisionFilterMask(TAG_TYPE tag)
{
	CollisionFilter = tag;
}

const TAG_TYPE DynamicColliderComponent::GetCollisionFilterMask()
{
	return CollisionFilter;
}

void DynamicColliderComponent::SetCollisionMode(CollisionFilterMode mode)
{
	collisionFilterMode = mode;
}

const CollisionFilterMode DynamicColliderComponent::GetCollisionFilterMode()
{
	return collisionFilterMode;
}

void DynamicColliderComponent::SetOnCollisionCallback(std::function<OnCollisionChoice(DynamicColliderComponent*)> callback)
{
	CollisionCallback = callback;
}
