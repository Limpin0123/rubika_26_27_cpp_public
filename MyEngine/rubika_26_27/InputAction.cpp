#include "InputAction.h"
#include <Imgui/imgui.h>

InputAction::InputAction(const std::string& name)
{
	SetActionName(name);
	Value = 0;
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
}

void InputAction::SetValue(float value)
{
	if (Value != value)
	{
		Value = value;
		for(sCallbackInfo& callbackInfo : Callbacks)
		{
			callbackInfo.Callback(Value);
		}
	}
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	Callbacks.push_back(sCallbackInfo(debugName, callback, owner));
}

void InputAction::UnregisterCallbacks(void* owner)
{
	for (int i = Callbacks.size() - 1; i >= 0; i--)
	{
		if (Callbacks[i].Owner == owner)
		{
			Callbacks.erase(Callbacks.begin() + i);
		}
	}
}

const std::string InputAction::GetName() const
{
	return ActionName;
}

const float InputAction::GetValue() const
{
	return Value;
}

void InputAction::DrawDebug()
{
	ImGui::Text(("Registered Callback(s): " + std::to_string(Callbacks.size())).c_str());
	for (sCallbackInfo& callBackInfo : Callbacks)
	{
		ImGui::Text(callBackInfo.DebugName.c_str());
	}
}
