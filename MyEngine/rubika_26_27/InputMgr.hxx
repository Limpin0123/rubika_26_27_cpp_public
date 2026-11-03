#include "InputMgr.h"
template<typename T>
void InputMgr::BindIAToKey(T key, std::unordered_map<T, std::set<int>>& map, const std::string& IA)
{
	auto it = map.find(key);
	if (it != map.end())//key already registered in bindings
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
		if (IANameToHash.contains(IA))
		{
			auto it2 = map.emplace(key, std::set<int>());
			it2.first->second.emplace(IANameToHash.at(IA));
		}
	}
}

template<typename T>
void InputMgr::UnbindIAFromKey(T key, std::unordered_map<T, std::set<int>>& map, const std::string& IA)
{
	auto it = map.find(key);
	if (it != map.end())//key registered in bindings
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
