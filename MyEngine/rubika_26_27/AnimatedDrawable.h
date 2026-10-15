#pragma once

#include "Globals.h"
#include "IDrawable.h"
#include <SFML/Graphics/RectangleShape.hpp>

struct TextureData;
struct AnimationData;

class AnimatedDrawable : public IDrawable
{
public:
	AnimatedDrawable();
	~AnimatedDrawable();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;


	virtual void DrawDebug() override;

	void SetAnimationRevertType(AnimationRevertType type);
	bool SetTexture(const std::string& textureName);
	bool SetAnimation(const std::string& animationName);
	bool SetTextureAndAnimation(const std::string& textureName, const std::string& animationName);

private:
	sf::RectangleShape rec;

	AnimationRevertType revertType;

	float animationTime;
	float color[4] = { 1,1,1,1 };
	const TextureData* textureData;
	const AnimationData* animationData;

	void CalculateAnimationSprite();
};

