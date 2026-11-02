#include <Imgui/imgui.h>
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
}

void InputMgr::Update(float fDelatTime)
{
	for (auto& binding : Bindings)
	{
		for (int IAHash : binding.second)
		{
			IAHashToClass.at(IAHash)->SetValue( (float)sf::Keyboard::isKeyPressed(binding.first) );
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

void InputMgr::BindIAToKey(sf::Keyboard::Key key, const std::string& IA)
{
	auto it = Bindings.find(key);
	if (it != Bindings.end())//key already registered in bindings
	{
		auto it2 = IANameToHash.find(IA);
		if (it2 == IANameToHash.end())
		{
			Logger::Error("Key not registered !");
			return;
		}

		it->second.emplace(it2->second);
	}
	else
	{
		auto it2 = Bindings.emplace(key, std::set<int>());
		it2.first->second.emplace(IANameToHash.at(IA));
	}
}
void InputMgr::UnbindIAFromKey(sf::Keyboard::Key key, const std::string& IA)
{
	auto it = Bindings.find(key);
	if (it != Bindings.end())//key registered in bindings
	{
		auto it2 = IANameToHash.find(IA);
		if (it2 == IANameToHash.end())
		{
			Logger::Error("Key not registered !");
			return;
		}

		it->second.erase(it2->second);
	}
}

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	ImGui::Text("Input Manager DebugWindow");

	if (ImGui::BeginTable("Bindings", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Key code");
		ImGui::TableSetupColumn("Bind Input Action(s)");
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		for (auto& binding : Bindings)
		{
			ImGui::TableSetColumnIndex(0);
			std::string keyName = sf::Keyboard::getDescription(sf::Keyboard::delocalize(binding.first)).toAnsiString();
			ImGui::Text(keyName.c_str());

			ImGui::TableSetColumnIndex(1);
			for (int hash : binding.second)
			{
				ImGui::Text(IAHashToClass.at(hash)->GetName().c_str());
			}

			ImGui::TableNextRow();
		}
	}
	ImGui::EndTable();

	if (ImGui::BeginTable("Input Actions", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Input Action Name");
		ImGui::TableSetupColumn("Input Action Hash");
		ImGui::TableHeadersRow();
		ImGui::TableNextRow();

		for (auto& InputActionPair : IANameToHash)
		{
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(InputActionPair.first.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::Text(std::to_string(InputActionPair.second).c_str());

			ImGui::TableSetColumnIndex(2);
			IAHashToClass.at(InputActionPair.second)->DrawDebug();

			ImGui::TableNextRow();
		}
	}
	ImGui::EndTable();
	
#endif // _USE_IMGUI

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
