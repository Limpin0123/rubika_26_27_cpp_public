#include "TextureMgr.h"
#include "Logger.h"

#pragma region TextureData
TextureData::TextureData()
{
}

TextureData::~TextureData()
{
}

#pragma endregion


#pragma region TextureMgr
void TextureMgr::Init()
{

}
void TextureMgr::Shut()
{

}

/*
* Loads the texture using the given path, including its metadata.
* Obviously, this function will also store the loaded data
* If errors occur during the loading process, this function must return false.
* Return true otherwise.
*/
bool TextureMgr::LoadTexture(const std::filesystem::path& imagePath)
{
	std::filesystem::path metaDataPath = imagePath;
	metaDataPath.replace_extension(".xml");

	if (!DoFilesExists(imagePath, metaDataPath))
	{
		return false;
	}
	
	rapidxml::xml_document<> document;
	rapidxml::file<> xml_file(metaDataPath.string().c_str());
	document.parse<0>(xml_file.data());


	return LoadTextureData(document, imagePath);
}

/*
* Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
*/
const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	if (!loadedTextures.contains(name))
		return TextureData();
	return loadedTextures.at(name);
}

bool TextureMgr::DoFilesExists(const std::filesystem::path& imagePath, const std::filesystem::path& metaDataPath)
{
	if (!std::filesystem::exists(imagePath))
	{
		Logger::Error("Image at path doesn't exists: " + imagePath.string());
		return false;
	}
	if (!std::filesystem::exists(metaDataPath))
	{
		Logger::Error("metaData at path doesn't exists: " + metaDataPath.string());
		return false;
	}
	return true;
}
bool TextureMgr::LoadTextureData(rapidxml::xml_document<>& doc, const std::filesystem::path& imagePath)
{
	rapidxml::xml_node<>* rootNode = doc.first_node();
		
	if (strcmp(rootNode->name(), "Animations") != 0 && strcmp(rootNode->name(), "Background") != 0)
	{
		Logger::Error("The XML file isn't of 'Animations' type nor 'Background'");
		return false;
	}
	rapidxml::xml_node<>* currentNode = rootNode->first_node();

	TextureData newTextureData = TextureData();

	newTextureData.Texture = sf::Texture(imagePath);


	while (currentNode != nullptr)
	{
		if (strcmp(rootNode->name(), "Animations") == 0)
		{
			auto it = newTextureData.animatedTextures.emplace(currentNode->first_attribute()->value(), AnimationData());
			AnimationData& ad = it.first->second;
			LoadAD(ad, currentNode->first_node());
		}
		else
		{
			auto it = newTextureData.textures.emplace(currentNode->first_attribute()->value(), StaticTileData());
			StaticTileData& std = it.first->second;
			LoadSTD(std, currentNode->first_node());
		}

		currentNode = currentNode->next_sibling();
	}

	auto it = loadedTextures.emplace(imagePath.filename().string(), TextureData());
	it.first->second = newTextureData;

	return true;
}

void  TextureMgr::LoadAD(AnimationData& ad, rapidxml::xml_node<>* node)
{
	if (!LoadIntAndNext(ad.StartX, node, "X"))
	{
		Logger::Error("Exeption at StartX");
		return;
	}
	if(!LoadIntAndNext(ad.StartY, node, "Y"))
	{
		Logger::Error("Exeption at StartY");
		return;
	}
	if (!LoadIntAndNext(ad.SizeX, node, "SizeX"))
	{
		Logger::Error("Exeption at SizeX");
		return;
	}
	if(!LoadIntAndNext(ad.SizeY, node, "SizeY"))
	{
		Logger::Error("Exeption at SizeY");
		return;
	}
	if (!LoadIntAndNext(ad.OffsetX, node, "OffsetX"))
	{
		Logger::Error("Exeption at OffsetX");
		return;
	}
	if (!LoadIntAndNext(ad.OffsetY, node, "OffsetY"))
	{
		Logger::Error("Exeption at OffsetY");
		return;
	}
	if (!LoadIntAndNext(ad.AnimationSpriteCount, node, "SpriteNum"))
	{
		Logger::Error("Exeption at SpriteNum");
		return;
	}
	if (!LoadIntAndNext(ad.SpriteOnLine, node, "SpritesOnLine"))
	{
		Logger::Error("Exeption at SpritesOnLine");
		return;
	}
	if (!LoadBoolAndNext(ad.IsReverted, node, "Reverted"))
	{
		Logger::Error("Exeption at Reverted");
		return;
	}
	if (!LoadFloatAndNext(ad.TimeBetweenAnimationInS, node, "TimeBetweenAnimation"))
	{
		Logger::Error("Exeption at TimeBetweenAnimation");
		return;
	}
		
}
void TextureMgr::LoadSTD(StaticTileData& std, rapidxml::xml_node<>* node)
{
	if(!LoadIntAndNext(std.StartX, node, "X"))
	{
		Logger::Error("Exeption at StartX");
		return;
	}
	if (!LoadIntAndNext(std.StartY, node, "Y"))
	{
		Logger::Error("Exeption at StartY");
		return;
	}
	if (!LoadIntAndNext(std.SizeX, node, "SizeX"))
	{
		Logger::Error("Exeption at SizeX");
		return;
	}
	if (!LoadIntAndNext(std.SizeY, node, "SizeY"))
	{
		Logger::Error("Exeption at SizeY");
		return;
	}
	if (!LoadBoolAndNext(std.IsRevertedX, node, "RevertedX"))
	{
		Logger::Error("Exeption at RevertedX");
		return;
	}
	if (!LoadBoolAndNext(std.IsRevertedY, node, "RevertedY"))
	{
		Logger::Error("Exeption at RevertedY");
		return;
	}
}

bool TextureMgr::LoadIntAndNext(int& i, rapidxml::xml_node<>*& node, const char* parameterName)
{
	if (node == nullptr)
	{
		Logger::Error("Error while parsing an XML file: node is null");
		return false;
	}
	if (strcmp(node->name(), parameterName) != 0)
	{
		Logger::Error("Error while parsing an XML file: name is invalid");
		return false;
	}
	i = std::strtol(node->value(), nullptr, 10);
	node = node->next_sibling();
	return true;
}
bool TextureMgr::LoadFloatAndNext(float& f, rapidxml::xml_node<>*& node, const char* parameterName)
{
	if (node == nullptr)
	{
		Logger::Error("Error while parsing an XML file: node is null");
		return false;
	}
	if (strcmp(node->name(), parameterName) != 0)
	{
		Logger::Error("Error while parsing an XML file: name is invalid");
		return false;
	}
	f = std::strtof(node->value(), nullptr);
	node = node->next_sibling();
	return true;
}
bool TextureMgr::LoadBoolAndNext(bool& b, rapidxml::xml_node<>*& node, const char* parameterName)
{
	if (node == nullptr)
	{
		Logger::Error("Error while parsing an XML file: node is null");
		return false;
	}
	if (strcmp(node->name(), parameterName) != 0)
	{
		Logger::Error("Error while parsing an XML file: name is invalid");
		return false;
	}
	b = node->value()[0] == '1';
	node = node->next_sibling();
	return true;
}
#pragma endregion

