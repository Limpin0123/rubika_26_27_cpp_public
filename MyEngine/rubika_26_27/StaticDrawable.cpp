#include "StaticDrawable.h"
#include "Globals.h"
#include "Logger.h"
#include "TextureMgr.h"

#include <SFML/Graphics/Rect.hpp>

#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#endif // _USE_IMGUI



StaticDrawable::StaticDrawable()
{}

StaticDrawable::~StaticDrawable()
{}

void StaticDrawable::Start()
{
	rec = sf::RectangleShape();
	rec.setFillColor(sf::Color(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255));
	rec.setSize(sf::Vector2f(100, 100));

	Drawable = &rec;
}

void StaticDrawable::Update(float deltaTime)
{

}

void StaticDrawable::Destroy()
{
	delete textureData;
}

void StaticDrawable::DrawDebug()
{
	IDrawable::DrawDebug();

	ImGui::PushID(this);
	if (ImGui::ColorPicker4("Fill Color", color))
	{
		sf::RectangleShape* rec = dynamic_cast<sf::RectangleShape*>(Drawable);
		rec->setFillColor(
			sf::Color(
				color[0] * 255,
				color[1] * 255,
				color[2] * 255,
				color[3] * 255
			)
		);
	}
	ImGui::PopID();
}

bool StaticDrawable::SetTexture(const std::string& textureName)
{
	textureData = &gData.TextureManager->GetTextureData(textureName);
	if (textureData == nullptr)
	{
		Logger::Warning("Couldn't load Texture");
		return false;
	}
	rec.setTexture(&textureData->Texture);

	return true;
}

bool StaticDrawable::SetStaticTile(const std::string& staticTileName)
{
	const StaticTileData* std = textureData->GetStaticTileData(staticTileName);
	if (std == nullptr)
	{
		Logger::Warning("Couldn't load staticTileData from texture");
		return false;
	}

	rec.setTextureRect(
		sf::IntRect(
			sf::Vector2i(
				std->IsRevertedX ? std->StartX + std->SizeX : std->StartX,
				std->IsRevertedY ? std->StartY + std->SizeY : std->StartY),
			sf::Vector2i(
				std->IsRevertedX ? -std->SizeX : std->SizeX,
				std->IsRevertedY ? -std->SizeY : std->SizeY)
		));
	return true;
}

bool StaticDrawable::SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName)
{
	return SetTexture(textureName) && SetStaticTile(staticTileName);
}
