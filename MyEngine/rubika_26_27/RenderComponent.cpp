#include "RenderComponent.h"

#include "StaticDrawable.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{
	//Visual = new StaticDrawable();
}

RenderComponent::~RenderComponent()
{}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (IDrawable* drawable : drawables)
	{
		drawable->Draw(window);
	}
}

void RenderComponent::Start()
{
	//Visual->Start();

	//Visual->SetWorldTransform(sf::Transform());
	//Visual->SetVisibility(true);
}

void RenderComponent::Update(float fDelatTime)
{
	for (IDrawable* drawable : drawables)
	{
		drawable->Update(fDelatTime);
	}
}

void RenderComponent::Destroy()
{
	for (IDrawable* drawable : drawables)
	{
		drawable->Destroy();
	}
}
