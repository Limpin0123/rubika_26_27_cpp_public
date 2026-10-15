#include "IDrawable.h"

#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#endif // _USE_IMGUI


IDrawable::IDrawable() : Visible(true), WorldTransform(sf::Transform::Identity), Drawable(nullptr)
{}

IDrawable::~IDrawable()
{}

bool IDrawable::IsVisible() const
{
	return Visible;
}

void IDrawable::SetVisibility(bool visible)
{
	Visible = visible;
}

const sf::Transform& IDrawable::GetWorldTransform() const
{
	return WorldTransform;
}

void IDrawable::SetWorldTransform(const sf::Transform& transform)
{
	WorldTransform = transform;
}

const sf::Transform& IDrawable::GetLocalTransform() const
{
	return LocalTransform;
}

void IDrawable::SetLocalTransform(const sf::Transform& transform)
{
	LocalTransform = transform;
}

void IDrawable::Draw(sf::RenderWindow& window) const
{
	if (!Drawable || !Visible)
	{
		return;
	}

	sf::RenderStates states = sf::RenderStates::Default;
	states.transform = WorldTransform;

	window.draw(*Drawable, states);
}

void IDrawable::DrawDebug()
{
	ImGui::PushID(this);
	ImGui::Text("drawable type: %s", typeid(*this).name());
	if (ImGui::Checkbox("Visibile", &Visible))
	{
		//nothing
	}
	ImGui::PopID();
}