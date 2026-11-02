#pragma once

#include <unordered_map>
#include <set>
#include "DebugMgr.h"
#include <SFML/Window/Keyboard.hpp>

class InputAction;

class InputMgr : public IDebugable
{

public:
	void Init();
	void Update(float fDelatTime);
	void Shut();

	void AddInputAction(InputAction*& inputAction);
	void RemoveInputAction(const std::string& name);

	void RegisterToIA(const std::string& IAName, const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnregisterFromIA(const std::string& IAName, void* owner);

	void BindIAToKey(sf::Keyboard::Key key, const std::string& IA);
	void UnbindIAFromKey(sf::Keyboard::Key key, const std::string& IA);

	virtual void DrawDebug() override;

private:
	std::unordered_map<int, InputAction*> IAHashToClass;
	std::unordered_map<std::string, int> IANameToHash;

	std::unordered_map<sf::Keyboard::Key, std::set<int>> Bindings;

	InputAction* GetIAByName(const std::string& IAName);
};