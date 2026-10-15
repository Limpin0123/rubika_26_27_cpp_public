#pragma once

#include "IComponent.h"
#include "IDrawableComponent.h"
#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable;

class RenderComponent : public IComponent, public IDrawableComponent
{

public:

	RenderComponent() = delete;
	RenderComponent(Entity& entity);
	~RenderComponent();

	template<typename T>
	T* AddDrawable();
	template<typename T>
	void RemoveDrawable(T* drawable);

	void Draw(sf::RenderWindow& window) const override;

	void Start() override;
	void Update(float fDelatTime) override;
	void Destroy() override;

	void DrawDebug() override;

private:
	std::vector<IDrawable*> drawables;
};

#include "RenderComponent.hxx"