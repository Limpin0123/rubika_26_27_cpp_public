#pragma once

#include <functional>
#include <string>
#include "DebugMgr.h"

class InputAction : public IDebugable
{
public:
	InputAction() = default;
	InputAction(const std::string& name);

	void SetActionName(const std::string& name);
	void SetValue(float value);

	void RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnregisterCallbacks(void* owner);

	const std::string GetName() const;
	const float GetValue() const;

	virtual void DrawDebug() override;

private:
	struct sCallbackInfo
	{
		std::string DebugName;
		std::function<void(float)> Callback;
		void* Owner = nullptr;

		sCallbackInfo() = default;
		sCallbackInfo(const std::string& name, std::function<void(float)> callback, void*& owner)
		{
			DebugName = name;
			Callback = callback;
			Owner = owner;
		}
	};

	std::string ActionName;
	std::size_t ActionNameHash;
	float Value = 0.f;
	std::vector<sCallbackInfo> Callbacks;
};