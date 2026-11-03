#include <filesystem>
#include <Imgui/imgui.h>

#include <rapidxml/rapidxml_utils.hpp>

#include "Logger.h"
#include "InputMgr.h"
#include "InputAction.h"
#include "Globals.h"
#include "SFML/System/String.hpp"


void InputMgr::Init()
{
#ifdef _USE_IMGUI
	gData.DebugMgr->RegisterDebugableWindow("Input Manager", this);
#endif // _USE_IMGUI
}
void InputMgr::Shut()
{
#ifdef _USE_IMGUI
	gData.DebugMgr->UnregisterDebugableWindow("Input Manager");
#endif // _USE_IMGUI

	for (auto& binding : IAHashToClass)
	{
		delete binding.second;
	}
	IAHashToClass.clear();
	IANameToHash.clear();
	BindingsKeyboard.clear();
	BindingsMouse.clear();
}

void InputMgr::Update(float fDelatTime)
{
	for (auto& binding : BindingsKeyboard)
	{
		for (int IAHash : binding.second)
		{
			IAHashToClass.at(IAHash)->SetValue( (float)sf::Keyboard::isKeyPressed(binding.first) );
		}
	}
	for (auto& binding : BindingsMouse)
	{
		for (int IAHash : binding.second)
		{
			IAHashToClass.at(IAHash)->SetValue((float)sf::Mouse::isButtonPressed(binding.first));
		}
	}
	for (auto& binding : BindingsCustom)
	{
		switch (binding.first)
		{
		case CustomBindings::MousePositionX:
			for (int IAHash : binding.second)
			{
				IAHashToClass.at(IAHash)->SetValue((float)sf::Mouse::getPosition().x);
			}
			break;
		case CustomBindings::MousePositionY:
			for (int IAHash : binding.second)
			{
				IAHashToClass.at(IAHash)->SetValue((float)sf::Mouse::getPosition().y);
			}
			break;
		default:
			break;
		}
	}
	for (auto& binding : BindingsJoystick)
	{
		for (int IAHash : binding.second)
		{
			//dividing by 100 to get value between -1 and 1
			IAHashToClass.at(IAHash)->SetValue((float)sf::Joystick::getAxisPosition(0,binding.first) * 0.01f);
		}
	}
}

void InputMgr::AddInputAction(InputAction*& inputAction)
{
	const std::string actionName = inputAction->GetName();
	int hash = std::hash<std::string>{} (actionName);

	auto it = IANameToHash.try_emplace(actionName, hash);
	if (it.second)//insertion succeeded
	{
		auto it2 = IAHashToClass.try_emplace(hash, inputAction);
	}
	else
	{
		Logger::Warning("Value: " + actionName + " was already present in inputActions and wasn't added a second time");
	}
}

void InputMgr::RemoveInputAction(const std::string& name)
{
	auto it = IANameToHash.find(name);
	if (it != IANameToHash.end())//element found
	{
		auto it2 = IAHashToClass.find(it->second);
		if (it2 != IAHashToClass.end())//element found
		{
			IAHashToClass.erase(it2);
		}
		IANameToHash.erase(it);
	}
}

void InputMgr::RegisterToIA(const std::string& IAName, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	InputAction* IA = GetIAByName(IAName);
	if (IA == nullptr)
	{
		Logger::Warning("Input Action: " + IAName + " wasn't found. Callback not added");
		return;
	}
	IA->RegisterCallback(debugName, callback, owner);
}

void InputMgr::UnregisterFromIA(const std::string& IAName, void* owner)
{
	InputAction* IA = GetIAByName(IAName);
	if (IA == nullptr)
	{
		Logger::Warning("Input Action: " + IAName + " wasn't found. Callback not removed");
		return;
	}
	IA->UnregisterCallbacks(owner);
}

void InputMgr::LoadBindings()
{
	LoadBindings(BINDINGS_FILE_PATH);
}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Bindings Map");

	if (ImGui::BeginTable("Bindings", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Key code");
		ImGui::TableSetupColumn("Bind Input Action(s)");
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		for (auto& binding : BindingsKeyboard)
		{
			std::function<void()> removeLambda = [this, binding]() -> void {
				auto it = BindingsKeyboard.find(binding.first);
				BindingsKeyboard.erase(it);
			};
			DrawDebugBinding(GetKeyName(binding.first), binding.second, removeLambda);
		}
		for (auto& binding : BindingsMouse)
		{
			std::function<void()> removeLambda = [this, binding]() -> void {
				auto it = BindingsMouse.find(binding.first);
				BindingsMouse.erase(it);
				};
			DrawDebugBinding(GetKeyName(binding.first), binding.second, removeLambda);
		}
		for (auto& binding : BindingsCustom)
		{
			std::function<void()> removeLambda = [this, binding]() -> void {
				auto it = BindingsCustom.find(binding.first);
				BindingsCustom.erase(it);
				};
			DrawDebugBinding(GetKeyName(binding.first), binding.second, removeLambda);
		}
		for (auto& binding : BindingsJoystick)
		{
			std::function<void()> removeLambda = [this, binding]() -> void {
				auto it = BindingsJoystick.find(binding.first);
				BindingsJoystick.erase(it);
				};
			DrawDebugBinding(GetKeyName(binding.first), binding.second, removeLambda);
		}
	}
	ImGui::EndTable();

	ImGui::SetNextItemWidth(150.0f);
	//Key dropdown
	static sf::Keyboard::Key currentKey = sf::Keyboard::Key::A;
	if (ImGui::BeginCombo("Keys", GetKeyName(currentKey).c_str()))
	{
		for (int i = 0; i <= (int)sf::Keyboard::Key::F12; i++)
		{
			sf::Keyboard::Key key = (sf::Keyboard::Key)i;
			const bool isSelected = currentKey == key;
			if (ImGui::Selectable(GetKeyName(key).c_str(), isSelected))
			{
				currentKey = key;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	//IA dropdown
	static std::string currentIA = IANameToHash.begin()->first;

	if (ImGui::BeginCombo("IA", currentIA.c_str()))
	{
		for (auto& IA : IANameToHash)
		{
			const bool isSelected = IANameToHash.at(currentIA) == IA.second;
			if (ImGui::Selectable(IA.first.c_str(), isSelected))
			{
				currentIA = IA.first;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);
	if (ImGui::Button("Add Binding"))
	{
		BindIAToKey<sf::Keyboard::Key>(currentKey, BindingsKeyboard, currentIA);
	}

	ImGui::Text("Input Actions Map");
	if (ImGui::BeginTable("Input Actions", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Input Action Name");
		ImGui::TableSetupColumn("Input Action Hash");
		ImGui::TableSetupColumn("Input Action Value");
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		for (auto& InputActionPair : IANameToHash)
		{
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(InputActionPair.first.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text(std::to_string(InputActionPair.second).c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text(std::to_string(IAHashToClass.at(InputActionPair.second)->GetValue()).c_str());

			ImGui::TableSetColumnIndex(3);
			IAHashToClass.at(InputActionPair.second)->DrawDebug();

			ImGui::TableNextRow();
		}
	}
	ImGui::EndTable();
	
#endif // _USE_IMGUI

}

void InputMgr::DrawDebugBinding(std::string keyName, std::set<int> bindingSet, std::function<void()> removeAction)
{
	ImGui::TableSetColumnIndex(0);
	ImGui::Text(keyName.c_str());

	ImGui::TableSetColumnIndex(1);
	for (int hash : bindingSet)
	{
		ImGui::Text(IAHashToClass.at(hash)->GetName().c_str());
	}

	ImGui::TableSetColumnIndex(2);
	if (ImGui::Button(("Remove Binding##" + keyName).c_str()))
	{
		removeAction();
		
		ImGui::EndTable();
		return;
	}

	ImGui::TableNextRow();
}

InputAction* InputMgr::GetIAByName(const std::string& IAName)
{
	auto it = IANameToHash.find(IAName);
	if (it != IANameToHash.end())
	{
		auto it2 = IAHashToClass.find(it->second);
		if (it2 != IAHashToClass.end())
		{
			return it2->second;
		}
		return nullptr;
	}
	return nullptr;
}

std::string InputMgr::GetKeyName(const sf::Keyboard::Key key) const
{
	return sf::Keyboard::getDescription(sf::Keyboard::delocalize(key)).toAnsiString();
}

std::string InputMgr::GetKeyName(const sf::Mouse::Button key) const
{
	switch (key)
	{
	case sf::Mouse::Button::Left:
		return "Mouse Left";
	case sf::Mouse::Button::Right:
		return "Mouse Right";
	case sf::Mouse::Button::Middle:
		return "Mouse Middle";
	case sf::Mouse::Button::Extra1:
		return "Mouse Extra1";
	case sf::Mouse::Button::Extra2:
		return "Mouse Extra2";
	default:
		return "Mouse Unknown";
	}
}

std::string InputMgr::GetKeyName(const sf::Joystick::Axis key) const
{
	switch (key)
	{
	case sf::Joystick::Axis::X:
		return "Joystick X";
	case sf::Joystick::Axis::Y:
		return "Joystick Y";
	case sf::Joystick::Axis::Z:
		return "Joystick Z";
	case sf::Joystick::Axis::R:
		return "Joystick R";
	case sf::Joystick::Axis::U:
		return "Joystick U";
	case sf::Joystick::Axis::V:
		return "Joystick V";
	case sf::Joystick::Axis::PovX:
		return "Joystick PovX";
	case sf::Joystick::Axis::PovY:
		return "Joystick PovY";
	default:
		return "Joystick Unknown";
	}
}

std::string InputMgr::GetKeyName(const CustomBindings key) const
{
	switch (key)
	{
	case CustomBindings::MousePositionX:
		return "Mouse position X";
	case CustomBindings::MousePositionY:
		return "Mouse position Y";
	default:
		return "Unknown Custom Binding";
	}
}

bool InputMgr::LoadBindings(const std::filesystem::path& bindingFilePath)
{
	if (!std::filesystem::exists(bindingFilePath))
	{
		Logger::Warning("Binding file couldn't be found");
		return false;
	}

	rapidxml::xml_document<> document;
	rapidxml::file<> xml_file(bindingFilePath.string().c_str());
	document.parse<0>(xml_file.data());

	rapidxml::xml_node<>* rootNode = document.first_node();
	if (strcmp(rootNode->name(), "Bindings") != 0)
	{
		Logger::Error("Binding file doesn't start with rootNode '<Bindings>'");
		return false;
	}

	rapidxml::xml_node<>* currentNode = rootNode->first_node();
	while (currentNode != nullptr)
	{
		LoadIABindings(currentNode);
		currentNode = currentNode->next_sibling();
	}

	return false;
}
bool InputMgr::LoadIABindings(rapidxml::xml_node<>*& InputActionNode)
{
	if (strcmp(InputActionNode->name(), "InputAction"))
	{
		Logger::Error("XML Node is not a '<InputAction>'");
		return false;
	}
	if (InputActionNode->first_attribute() == nullptr || strcmp(InputActionNode->first_attribute()->name(), "Name") != 0)
	{
		Logger::Error("XML Node '<InputAction>' doesn't contain attribute 'Name'");
		return false;
	}
	std::string IAName = InputActionNode->first_attribute()->value();

	rapidxml::xml_node<>* currentKeyNode = InputActionNode->first_node();
	while (currentKeyNode != nullptr)
	{
		if (strcmp(currentKeyNode->name(), "BindedKey") != 0 ||
			currentKeyNode->first_attribute() == nullptr ||
			strcmp(currentKeyNode->first_attribute()->name(), "Type") != 0)
		{
			Logger::Warning("'<BindedKey>' attribute 'Type' isn't present or valid, skipping");
			currentKeyNode = currentKeyNode->next_sibling();
			continue;
		}

		if (strcmp(currentKeyNode->first_attribute()->value(), "Keyboard") == 0)
		{
			BindIAToKey<sf::Keyboard::Key>((sf::Keyboard::Key)std::strtol(currentKeyNode->value(), nullptr, 10), BindingsKeyboard, IAName);
		}
		else if (strcmp(currentKeyNode->first_attribute()->value(), "Mouse") == 0)
		{
			if (strcmp(currentKeyNode->value(), "PositionX") == 0)
			{
				BindIAToKey<CustomBindings>(CustomBindings::MousePositionX, BindingsCustom, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "PositionY") == 0)
			{
				BindIAToKey<CustomBindings>(CustomBindings::MousePositionY, BindingsCustom, IAName);
			}
			else
			{
				BindIAToKey<sf::Mouse::Button>((sf::Mouse::Button)std::strtol(currentKeyNode->value(), nullptr, 10), BindingsMouse, IAName);
			}
		}
		else if (strcmp(currentKeyNode->first_attribute()->value(), "Joystick") == 0)
		{
			if (strcmp(currentKeyNode->value(), "X") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::X, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "Y") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::Y, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "Z") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::Z, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "R") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::R, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "U") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::U, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "V") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::V, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "PovX") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::PovX, BindingsJoystick, IAName);
			}
			else if (strcmp(currentKeyNode->value(), "PovY") == 0)
			{
				BindIAToKey<sf::Joystick::Axis>(sf::Joystick::Axis::PovY, BindingsJoystick, IAName);
			}
		}
		
		currentKeyNode = currentKeyNode->next_sibling();
	}
	return true;
}
