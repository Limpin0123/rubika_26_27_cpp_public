#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"


#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#include "StaticDrawable.h"
#include "AnimatedDrawable.h"
#endif // _USE_IMGUI


RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{

}

RenderComponent::~RenderComponent()
{}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	TransformComponent* tr = GetEntity().GetComponent<TransformComponent>();
	if (tr == nullptr)
	{
		return;
	}

	for (IDrawable* drawable : drawables)
	{
		sf::Transform wTr = sf::Transform();

		wTr.rotate(sf::degrees(tr->GetRotation()))
			.translate(tr->GetWorldPosition());

		drawable->SetWorldTransform(wTr);
		drawable->Draw(window);
	}
}

void RenderComponent::Start()
{
	
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

void RenderComponent::DrawDebug()
{
	ImGui::BeginGroup();
	ImGui::TextColored(ImVec4(255, 0, 0, 255), "Render Component");

	if (ImGui::Button("Add Static Drawable"))
	{
		AddDrawable<StaticDrawable>();
	}
	if (ImGui::Button("Add Animated Drawable"))
	{
		AddDrawable<AnimatedDrawable>();
	}

	for (IDrawable* drawable : drawables)
	{
		drawable->DrawDebug();
		ImGui::PushID(drawable);
		if (ImGui::Button("Remove"))
		{
			RemoveDrawable(drawable);
		}
		ImGui::PopID();
		ImGui::NewLine();
	}

	ImGui::EndGroup();
}
