#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

struct AnimationData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	int OffsetX = 0;
	int OffsetY = 0;
	int AnimationSpriteCount = 0;
	int SpriteOnLine = 0;
	bool IsReverted = false;
	float TimeBetweenAnimationInS = 0.f;
};

struct StaticTileData
{
	int StartX = 0;
	int StartY = 0;
	int SizeX = 0;
	int SizeY = 0;
	bool IsRevertedX = false;
	bool IsRevertedY = false;
};

struct TextureData
{
	TextureData();
	~TextureData();

	sf::Texture Texture;

	// Container of AnimationData AnimationsData;
	std::unordered_map<std::string, AnimationData> animatedTextures;
	// Container of StaticTileData StaticTilesData;
	std::unordered_map<std::string, StaticTileData> textures;

	friend class TextureMgr;
};

class TextureMgr final
{
public:
	void Init();
	void Shut();

	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& imagePath);

	/*
	 * Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
	 */
	const TextureData& GetTextureData(const std::string& name) const;

private:
	std::unordered_map<std::string, TextureData> loadedTextures;

	bool DoFilesExists(const std::filesystem::path& imagePath, const std::filesystem::path& metaDataPath);

	bool LoadTextureData(rapidxml::xml_document<>& doc, const std::filesystem::path& imagePath);
	
	void LoadAD(AnimationData& ad, rapidxml::xml_node<>* node);
	void LoadSTD(StaticTileData& std, rapidxml::xml_node<>* node);

	bool LoadIntAndNext(int& i, rapidxml::xml_node<>*& node, const char* parameterName);
	bool LoadFloatAndNext(float& f, rapidxml::xml_node<>*& node, const char* parameterName);
	bool LoadBoolAndNext(bool& b, rapidxml::xml_node<>*& node, const char* parameterName);
};