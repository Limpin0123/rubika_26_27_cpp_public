#include "StaticDrawable.h"
#include "Globals.h"
#include "TextureMgr.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

StaticDrawable::StaticDrawable()
{
}

StaticDrawable::~StaticDrawable()
{
}

void StaticDrawable::Start()
{
	sf::RectangleShape* rec = new sf::RectangleShape();
	rec->setFillColor(sf::Color(255, 0, 0, 255));
	rec->setSize(sf::Vector2f(500, 500));
	rec->setPosition(sf::Vector2f(10, 10));
	rec->setTexture(&gData.TextureManager->GetTextureData(DEFAULT_TEXTURE_NAME).Texture);

	Drawable = rec;
}

void StaticDrawable::Update(float deltaTime)
{

}

void StaticDrawable::Destroy()
{
}
