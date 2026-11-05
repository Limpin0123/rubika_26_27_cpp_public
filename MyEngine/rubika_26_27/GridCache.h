#pragma once

#include <SFML/Graphics/Rect.hpp>
#ifdef _DEBUG
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Globals.h"
#include "DebugMgr.h"
#endif // _DEBUG

#include <vector>

class DynamicColliderComponent;

class ColliderGridCache
{
public:
	ColliderGridCache(sf::Vector2i posRange, int subdivideAmount);

	void AddCollider(DynamicColliderComponent* collider);
	void RemoveCollider(DynamicColliderComponent* collider);
	void GetColliders(std::vector<DynamicColliderComponent*>* colliders, DynamicColliderComponent* collider);

	void clear();

#ifdef _DEBUG
	sf::Font debugFont;
	void Draw(sf::RenderWindow& window) const;
	inline static bool showGrid = true;
#endif // _DEBUG


private:
	//lines || columns || entities in grid
	std::vector< std::vector< std::vector<DynamicColliderComponent*>>> grid;
	std::vector<DynamicColliderComponent*> outOfGrid;

	sf::Vector2i positionRange;
	int cellSize;
	int gridSize;

	void GetCellsFromBounds(std::vector<std::vector<DynamicColliderComponent*>*>* container, DynamicColliderComponent* collider);
	std::pair<int, int> GetCellFromPosition(float x, float y);

	float clamp(float value, float min, float max);
};

