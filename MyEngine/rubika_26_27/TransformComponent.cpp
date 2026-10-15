#include "TransformComponent.h"
#include "Entity.h"
#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#endif // _USE_IMGUI


TransformComponent::TransformComponent(Entity& entity) : IComponent(entity)
{
	WorldPosition = sf::Vector2f();
	WorldPositionQuery = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}


TransformComponent::~TransformComponent()
{}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	WorldPositionQuery = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetWorldPosition() const
{
	return WorldPosition;
}

void TransformComponent::SetWorldPositionQuery(sf::Vector2f newPosition)
{
	WorldPositionQuery = newPosition;
}

sf::Vector2f TransformComponent::GetWorldPositionQuery() const
{
	return WorldPositionQuery;
}

void TransformComponent::SetRotation(float newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

float TransformComponent::GetRotation() const
{
	return Rotation;
}

void TransformComponent::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetScale() const
{
	return Scale;
}



void TransformComponent::Start()
{}

void TransformComponent::Update(float fDeltaTime)
{}

void TransformComponent::Destroy()
{}

void TransformComponent::DrawDebug()
{
	int step = 1;
	int fastStep = 10;
	int flags = ImGuiInputTextFlags_CharsDecimal;


	ImGui::BeginGroup();
	ImGui::TextColored(ImVec4(255, 0, 0, 255), "Transform Component");
	ImGui::InputFloat2("Position: {X,Y}", &WorldPosition.x, "%.3f", flags);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		UpdateMatrix();
	}
	ImGui::NewLine();
	ImGui::InputFloat("Rotation", &Rotation, step, fastStep, "%.3f", flags);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		UpdateMatrix();
	}
	ImGui::NewLine();
	ImGui::InputFloat2("Scale {X,Y}", &Scale.x, "%.3f", flags);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		UpdateMatrix();
	}
	ImGui::EndGroup();
}

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}