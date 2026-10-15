#pragma once

#include "IDrawable.h"
#include <SFML/Graphics/RectangleShape.hpp>

struct TextureData;

class StaticDrawable : public IDrawable
{
public: 
	StaticDrawable();
	~StaticDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void DrawDebug() override;

	bool SetTexture(const std::string& textureName);
	bool SetStaticTile(const std::string& staticTileName);
	bool SetTextureAndStaticTile(const std::string& textureName, const std::string& staticTileName);

private:
	sf::RectangleShape rec;

	float color[4] = { 1,1,1,1 };
	const TextureData* textureData;
};

