#pragma once

#include <unordered_map>
#include <set>
#include "DebugMgr.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>

#include <rapidxml/rapidxml.hpp>

#define BINDINGS_FILE_PATH "C:\\Github\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\Bindings.xml"

enum CustomBindings
{
	MousePositionX,
	MousePositionY
};

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

	template<typename T>
	void BindIAToKey(T key, std::unordered_map<T, std::set<int>>& map,  const std::string& IA);
	template<typename T>
	void UnbindIAFromKey(T key, std::unordered_map<T, std::set<int>>& map, const std::string& IA);

	void LoadBindings();

	virtual void DrawDebug() override;

private:
	std::unordered_map<int, InputAction*> IAHashToClass;
	std::unordered_map<std::string, int> IANameToHash;

	std::unordered_map<sf::Keyboard::Key, std::set<int>> BindingsKeyboard;
	std::unordered_map<sf::Mouse::Button, std::set<int>> BindingsMouse;
	std::unordered_map<sf::Joystick::Axis, std::set<int>> BindingsJoystick;
	std::unordered_map<CustomBindings, std::set<int>> BindingsCustom;


	InputAction* GetIAByName(const std::string& IAName);
	std::string GetKeyName(const sf::Keyboard::Key key) const;
	std::string GetKeyName(const sf::Mouse::Button key) const;
	std::string GetKeyName(const sf::Joystick::Axis key) const;
	std::string GetKeyName(const CustomBindings key) const;

	bool LoadBindings(const std::filesystem::path& bindingFilePath);
	bool LoadIABindings(rapidxml::xml_node<>*& InputActionNode);

	void DrawDebugBinding(std::string keyName, std::set<int> bindingSet, std::function<void()> removeAction);
};

#include "InputMgr.hxx"