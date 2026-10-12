#include "SceneMgr.h"

	#include "Logger.h"
	#include "Globals.h"
#ifdef _USE_IMGUI
	#include <Imgui/imgui.h>
#endif

SceneMgr SceneManager;

SceneMgr::SceneMgr()
{
	activeScene = nullptr;
	gData.DebugMgr->RegisterDebugableWindow("Scene Debugger", this);
}

SceneMgr::~SceneMgr()
{
	gData.DebugMgr->UnregisterDebugableWindow("Scene Debugger");
	for (auto& scenePair : scenes)
	{
		RemoveScene(scenePair.first);
	}
}

Scene* SceneMgr::AddAndLoadScene(std::string sceneName)
{
	auto* s = AddScene(sceneName);
	activeScene = s;
	return s;
}

Scene* SceneMgr::AddScene(std::string sceneName)
{
	auto result = scenes.emplace(sceneName, Scene(sceneName));
	if (!result.second)
	{
		Logger::Warning("A scene of with the same name already exists in the scene Manager (" + sceneName + ")");
		return nullptr;
	}

	result.first->second.Start();
	return &result.first->second;
}

void SceneMgr::RemoveScene(std::string sceneName)
{
	auto it = scenes.find(sceneName);
	if (it == scenes.end())
		return;

	if (activeScene == &it->second)
	{
		activeScene = nullptr;
	}
	it->second.Destroy();
	scenes.erase(sceneName);
}

void SceneMgr::LoadScene(std::string sceneName)
{
	auto it = scenes.find(sceneName);
	if(it == scenes.end())
		return;

	activeScene = &it->second;
}

void SceneMgr::UpdateActiveScene(float fDeltaTime)
{
	if (activeScene == nullptr)
		return;
	activeScene->Update(fDeltaTime);
}

void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	if (inspectedScene == nullptr)
	{
		static char newSceneName[128] = "Scene_01";
		if (ImGui::Button("Add Scene"))
		{
			AddScene(newSceneName);
		}
		if (ImGui::InputText("##inputText", newSceneName, IM_ARRAYSIZE(newSceneName)))
		{}

		if (ImGui::BeginTable("Scenes", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
		
			ImGui::TableSetupColumn("Scene Name");
			ImGui::TableSetupColumn("Entity amount");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			for (auto& scenePair : scenes)
			{
				ImVec4 color = &scenePair.second == activeScene ? ImVec4(1, 1, 0, 1) : ImVec4(1, 1, 1, 1);

				ImGui::TableSetColumnIndex(0);
				ImGui::TextColored(color, scenePair.first.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text(std::to_string(scenePair.second.GetEntityAmount()).c_str());

				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(("Inspect##" + scenePair.first).c_str()))
				{
					inspectedScene = &scenePair.second;
				}

				ImGui::TableSetColumnIndex(3);
				if (ImGui::Button(("SetActiveScene##" + scenePair.first).c_str()))
				{
					LoadScene(scenePair.first);
				}
				ImGui::TableSetColumnIndex(4);
				if (ImGui::Button(("Destroy##" + scenePair.first).c_str()))
				{
					RemoveScene(scenePair.first);
					ImGui::EndTable();
					return;
				}

				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
	}
	else
	{
		if (ImGui::Button("Back"))
		{
			inspectedScene = nullptr;
			return;
		}

		ImGui::SameLine();
		static char newEntityName[128] = "Entity_01";
		if (ImGui::Button("Add Entity"))
		{
			inspectedScene->AddEntity(newEntityName);
		}
		ImGui::SameLine();
		if(ImGui::InputText("##InputText", newEntityName, IM_ARRAYSIZE(newEntityName)))
		{}


		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Entities in scene:");
		if (ImGui::BeginTable("Entities", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			int BtnId = 0;
			for (Entity& entity : inspectedScene->GetEntities()) {
				
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(entity.name.c_str());

				ImGui::TableSetColumnIndex(1);
				if (ImGui::Button(("Remove##" + std::to_string(BtnId++)).c_str()))
				{
					inspectedScene->RemoveEntity(&entity);
					ImGui::EndTable();
					return;
				}

				ImGui::TableNextRow();
			}

			ImGui::EndTable();
		}
		
	}
#endif
}
