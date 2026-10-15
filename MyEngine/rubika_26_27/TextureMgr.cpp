#include "TextureMgr.h"
#include "Logger.h"

#ifdef _USE_IMGUI
//#include <filesystem>
#include "Globals.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif

#pragma region TextureData
TextureData::TextureData()
{
	
}

TextureData::~TextureData()
{
}

bool TextureData::isAnimatedData()
{
	return animatedTextures.size() > 0;
}

const StaticTileData* TextureData::GetStaticTileData(std::string textureName) const
{
	auto it = staticTextures.find(textureName);
	if (it == staticTextures.end())
		return nullptr;
	return &it->second;
}

const AnimationData* TextureData::GetAnimatedTileData(std::string textureName) const
{
	auto it = animatedTextures.find(textureName);
	if (it == animatedTextures.end())
		return nullptr;
	return &it->second;
}

#pragma endregion


#pragma region TextureMgr
void TextureMgr::Init()
{
	sf::Texture  tex(sf::Vector2u(16,16));
	std::vector<std::uint8_t> pixels(1024, 255);//1024 => 16*16*4
	tex.update(pixels.data());

	TextureData defaultTex = TextureData();
	defaultTex.Texture = tex;
	auto it = defaultTex.staticTextures.emplace("default", StaticTileData());
	StaticTileData& std = it.first->second;
	std.StartX = 0;
	std.StartY = 0;
	std.SizeX = 16;
	std.SizeY = 16;
	std.IsRevertedX = false;
	std.IsRevertedY = false;

	loadedTextures.emplace(DEFAULT_TEXTURE_NAME, defaultTex);


#ifdef _USE_IMGUI
	gData.DebugMgr->RegisterDebugableWindow("Texture Debugger", this);
#endif // _USE_ImGUI

}
void TextureMgr::Shut()
{
#ifdef _USE_IMGUI
	gData.DebugMgr->UnregisterDebugableWindow("Texture Debugger");
#endif // _USE_ImGUI
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
	{
		Logger::Warning("Couldn't load texture: " + name);
		return loadedTextures.at(DEFAULT_TEXTURE_NAME);
	}
	return loadedTextures.at(name);
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	if (inspectedTexture.empty())
	{
		static char newTextureName[128] = "";
		if (ImGui::Button("Load new Texture"))
		{
			std::string fileName(newTextureName);

			if (!fileName.empty())
			{
				GET_FULLPATH_WITH_FILENAME fileName;
				LoadTexture(fullPath.string());
			}
		}
		if(ImGui::InputText("##InputText", newTextureName, IM_ARRAYSIZE(newTextureName)))
		{}

		if (ImGui::BeginTable("Textures", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Texture name");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			for (std::pair<std::string, TextureData> texPair : loadedTextures)
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(texPair.first.c_str());

				ImGui::TableSetColumnIndex(1);
				if (ImGui::Button(("Inspect##" + texPair.first).c_str()))
				{
					inspectedTexture = texPair.first;
					ImGui::EndTable();
					return;
				}

				ImGui::TableNextRow();
			}
		}
		ImGui::EndTable();
	}
	else
	{
		if (ImGui::Button("Back"))
		{
			inspectedTexture.clear();
			return;
		}
		TextureData& tex = loadedTextures.at(inspectedTexture);
		ImVec4 titleColor = ImVec4(1,0,0,1);
		ImVec4 subTitleColor = ImVec4(0, 0, 1, 1);

		if (tex.isAnimatedData())
		{
			ImGui::TextColored(titleColor, "Animations");
			ImGui::NewLine();
			for (std::pair<std::string, AnimationData> adPair : tex.animatedTextures)
			{
				AnimationData& ad = adPair.second;
				
				ImGui::PushStyleColor(ImGuiCol_Text, subTitleColor);
				bool isOpen = ImGui::TreeNode(adPair.first.c_str());
				ImGui::PopStyleColor();

				if (isOpen)
				{
					ImGui::Text("Size {%d,%d} {%d,%d}", ad.StartX, ad.StartY, ad.SizeX, ad.SizeY);
					ImGui::Text("Offset {%d,%d}", ad.OffsetX, ad.OffsetY);
					ImGui::Text("Sprites: %d", ad.SpriteOnLine);
					ImGui::Text("Reverted: %s", ad.IsReverted ? "true" : "false");
					ImGui::Text("Sprite Delay: %f", ad.TimeBetweenAnimationInS);

					ImGui::TreePop();
				}

			}
		}
		else
		{
			ImGui::TextColored(titleColor, "Background");
			ImGui::NewLine();
			for (std::pair<std::string, StaticTileData> stdPair : tex.staticTextures)
			{
				StaticTileData& std = stdPair.second;

				ImGui::PushStyleColor(ImGuiCol_Text, subTitleColor);

				bool isOpen = ImGui::TreeNode(stdPair.first.c_str());
				ImGui::PopStyleColor();

				if (isOpen)
				{
					ImGui::Text("Size {%d,%d} {%d,%d}", std.StartX, std.StartY, std.SizeX, std.SizeY);
					ImGui::Text("Reverted X: %s  || Reverted Y: %s", std.IsRevertedX ? "true" : "false", std.IsRevertedY ? "true" : "false");

					ImGui::TreePop();
				}
			}
		}

		ImGui::NewLine();

		sf::Vector2 texSize(tex.Texture.getSize());
		ImVec2 freeSpace = ImGui::GetContentRegionAvail();
		float scale = texSize.x > freeSpace.x ? freeSpace.x / texSize.x : 1;
		sf::Vector2 size(texSize.x * scale, texSize.y * scale);

		ImGui::Image(tex.Texture, size);
	}
#endif // _USE_IMGUI
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
		
	if (strcmp(rootNode->name(), "Animations") != 0 && strcmp(rootNode->name(), "Backgrounds") != 0)
	{
		Logger::Error("The XML file isn't of 'Animations' type nor 'Backgrounds'");
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
			auto it = newTextureData.staticTextures.emplace(currentNode->first_attribute()->value(), StaticTileData());
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