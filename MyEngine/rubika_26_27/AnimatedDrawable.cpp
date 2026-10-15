#include "AnimatedDrawable.h"
#include "TextureMgr.h"
#include "Globals.h"
#include "Logger.h"
#include <cmath>

#ifdef _USE_IMGUI
#include "Imgui/imgui.h"

#endif // _USE_IMGUI

AnimatedDrawable::AnimatedDrawable()
{}

AnimatedDrawable::~AnimatedDrawable()
{
	delete textureData;
}

void AnimatedDrawable::Start()
{
	revertType = RevertX;

	rec = sf::RectangleShape();
	rec.setFillColor(sf::Color(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255));
	rec.setSize(sf::Vector2f(100, 100));

	Drawable = &rec;
}

void AnimatedDrawable::Update(float deltaTime)
{
	animationTime += deltaTime;
	CalculateAnimationSprite();
}

void AnimatedDrawable::Destroy()
{}

void AnimatedDrawable::DrawDebug()
{
	IDrawable::DrawDebug();
	ImGui::PushID(this);
	//revertType
	static int currentItem = revertType;
	const char* items[] = { "RevertAnimation", "RevertX", "RevertY", "RevertXY"};
	if(ImGui::Combo("RevertType", &currentItem, items, IM_ARRAYSIZE(items)))
	{
		AnimationRevertType art = (AnimationRevertType)currentItem;
		revertType = art;
	}
	ImGui::PopID();
}

void AnimatedDrawable::SetAnimationRevertType(AnimationRevertType type)
{
	revertType = type;
}

bool AnimatedDrawable::SetTexture(const std::string& textureName)
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

bool AnimatedDrawable::SetAnimation(const std::string& animationName)
{
	animationData = textureData->GetAnimatedTileData(animationName);
	if (animationData == nullptr)
	{
		Logger::Warning("Couldn't load animationData from texture");
		return false;
	}

	animationTime = 0;
	CalculateAnimationSprite();

	return true;
}

bool AnimatedDrawable::SetTextureAndAnimation(const std::string& textureName, const std::string& animationName)
{
	return SetTexture(textureName) && SetAnimation(animationName);
}

void AnimatedDrawable::CalculateAnimationSprite()
{
	int sIndex = static_cast<int>(animationTime / animationData->TimeBetweenAnimationInS) % animationData->AnimationSpriteCount;
	if (revertType == RevertAnimation && animationData->IsReverted)
	{
		sIndex = animationData->AnimationSpriteCount - 1 - sIndex;
	}
	int sIndexX = static_cast<int>(sIndex % animationData->SpriteOnLine);
	int sIndexY = static_cast<int>(sIndex / animationData->SpriteOnLine);

	int skipSizeX = animationData->SizeX + animationData->OffsetX;
	int skipSizeY = animationData->SizeY + animationData->OffsetY;

	bool IsRevertX = animationData->IsReverted && (revertType == RevertX || revertType == RevertXY);
	bool IsRevertY = animationData->IsReverted && (revertType == RevertY || revertType == RevertXY);

	sf::Vector2 position = sf::Vector2i(
		IsRevertX ? animationData->StartX + (skipSizeX * sIndexX) + animationData->SizeX : animationData->StartX + skipSizeX * sIndexX,
		IsRevertY ? animationData->StartY + (skipSizeY * sIndexY) + animationData->SizeY : animationData->StartY + skipSizeY * sIndexY
	);

	sf::Vector2 size = sf::Vector2i(
		IsRevertX ? -animationData->SizeX : animationData->SizeX,
		IsRevertY ? -animationData->SizeY : animationData->SizeY
	);

	rec.setTextureRect(sf::IntRect(position, size));
}
