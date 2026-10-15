#ifdef _USE_IMGUI
#include "Imgui/imgui.h"
#endif // _USE_IMGUI

template<typename T>
void SceneMgr::DrawComponentButton(Entity& entity, std::string title)
{
	bool canAddComponent = entity.CanAddComponent<T>();
	ImGui::PushStyleColor(ImGuiCol_Button, canAddComponent ? ImVec4(0.5,0.5,0.5,1) : ImVec4(0.2,0.2,0.2,1));
	if (ImGui::Button(title.c_str()))
	{
		if (canAddComponent)
		{
			entity.AddComponent<T>();
		}
		else
		{
			entity.RemoveComponent<T>();
		}
	}
	ImGui::PopStyleColor();
}