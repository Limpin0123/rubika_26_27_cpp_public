#include "RenderComponent.h"

#include "StaticDrawable.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{
	Visual = new StaticDrawable();
}

RenderComponent::~RenderComponent()
{}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	Visual->Draw(window);
}

void RenderComponent::Start()
{
	Visual->Start();

	Visual->SetWorldTransform(sf::Transform());
	Visual->SetVisibility(true);
}

void RenderComponent::Update(float fDelatTime)
{
	Visual->Update(fDelatTime);
}

void RenderComponent::Destroy()
{
	Visual->Destroy();
}
