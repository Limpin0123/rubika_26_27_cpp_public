#include "GridCache.h"
#include "DynamicColliderComponent.h"

ColliderGridCache::ColliderGridCache(sf::Vector2i posRange, int subdivideAmount)
{
	positionRange = posRange;
	int size = positionRange.y - positionRange.x;
	cellSize = subdivideAmount <= 0 ? size : size / subdivideAmount;

	gridSize = ceil((float)size / (float)cellSize);

	for (int lineIndex = 0; lineIndex < gridSize; lineIndex++)
	{
		std::vector<std::vector<DynamicColliderComponent*>> line;
		for (int columnIndex = 0; columnIndex < gridSize; columnIndex++)
		{
			std::vector<DynamicColliderComponent*> cell;
			line.push_back(cell);
		}
		grid.push_back(line);
	}

#ifdef _DEBUG
	debugFont.openFromFile("C:\\Github\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\LiberationSans.ttf");
	std::function<void(void*)> lambda = [](void*) -> void
		{
			ColliderGridCache::showGrid = !ColliderGridCache::showGrid;
		};
	gData.DebugMgr->RegisterDebugAction("Show Grid", lambda, this);
#endif // _DEBUG

}

void ColliderGridCache::AddCollider(DynamicColliderComponent* collider)
{
	std::vector<std::vector<DynamicColliderComponent*>*> cells;
	GetCellsFromBounds(&cells, collider);
	for (auto cell : cells)
	{
		cell->push_back(collider);
	}
}

void ColliderGridCache::RemoveCollider(DynamicColliderComponent* collider)
{
	std::vector<std::vector<DynamicColliderComponent*>*> cells;
	GetCellsFromBounds(&cells, collider);
	for (auto cell : cells)
	{
		//Find element in cell and removing it
		auto it = std::find(cell->begin(), cell->end(), collider);
		cell->erase(it);
	}
}

void ColliderGridCache::GetColliders(std::vector<DynamicColliderComponent*>* colliders, DynamicColliderComponent* collider)
{
	std::vector<std::vector<DynamicColliderComponent*>*> cells;
	GetCellsFromBounds(&cells, collider);

	for (auto cell = cells.begin(); cell != cells.end(); cell++)
	{
		for (auto collider = (*cell)->begin(); collider != (*cell)->end(); collider++)
		{
			colliders->push_back(*collider);
		}
	}
}

void ColliderGridCache::clear()
{
	for (int lineIndex = 0; lineIndex < gridSize; lineIndex++)
	{
		for (int columnIndex = 0; columnIndex < gridSize; columnIndex++)
		{
			grid.at(lineIndex).at(columnIndex).clear();
		}
		grid.at(lineIndex).clear();
	}
	grid.clear();
}

#ifdef _DEBUG
void ColliderGridCache::Draw(sf::RenderWindow& window) const
{
	if (!showGrid) return;
	sf::VertexArray lines(sf::PrimitiveType::Lines);

	float gridStart = positionRange.x;
	float gridEnd = positionRange.y;

	for (int lineIndex = 0; lineIndex < gridSize; lineIndex++)
	{
		for (int columnIndex = 0; columnIndex < gridSize; columnIndex++)
		{
			if (grid[lineIndex][columnIndex].size() <= 0) continue;

			float x = gridStart + (lineIndex + 0.5f) * cellSize;
			float y = gridStart + (columnIndex + 0.5f) * cellSize;

			//173 173 255
			//10 60 60
			float t = grid[lineIndex][columnIndex].size()/10;
			uint8_t red = std::lerp(173, 10, t);
			uint8_t green = std::lerp(173, 60, t);
			uint8_t blue = std::lerp(255, 60, t);

			sf::RectangleShape cellBg = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
			cellBg.setPosition(sf::Vector2f(x-cellSize*0.5f,y-cellSize * 0.5f));
			cellBg.setFillColor(sf::Color(red, green, blue, 180));

			sf::Text text(
				debugFont,
				std::to_string(grid[lineIndex][columnIndex].size()),
				16
			);

			auto bounds = text.getLocalBounds();
			text.setOrigin({
				bounds.position.x + bounds.size.x / 2.f,
				bounds.position.y + bounds.size.y / 2.f
				});

			text.setPosition({ x, y });
			text.setFillColor(sf::Color::White);

			window.draw(cellBg);
			window.draw(text);
		}

		float point = gridStart + lineIndex * cellSize;

		lines.append(sf::Vertex({ point, gridStart }, sf::Color::Green));
		lines.append(sf::Vertex({ point, gridEnd }, sf::Color::Green));

		lines.append(sf::Vertex({ gridStart, point }, sf::Color::Green));
		lines.append(sf::Vertex({ gridEnd, point }, sf::Color::Green));
	}

	sf::Color borderColor = sf::Color::Red;

	lines.append(sf::Vertex({ gridStart, gridStart }, borderColor));
	lines.append(sf::Vertex({ gridEnd, gridStart }, borderColor));

	lines.append(sf::Vertex({ gridEnd, gridStart }, borderColor));
	lines.append(sf::Vertex({ gridEnd, gridEnd }, borderColor));

	lines.append(sf::Vertex({ gridEnd, gridEnd }, borderColor));
	lines.append(sf::Vertex({ gridStart, gridEnd }, borderColor));

	lines.append(sf::Vertex({ gridStart, gridEnd }, borderColor));
	lines.append(sf::Vertex({ gridStart, gridStart }, borderColor));

	window.draw(lines);
}
#endif // _DEBUG

void ColliderGridCache::GetCellsFromBounds(std::vector<std::vector<DynamicColliderComponent*>*>* container, DynamicColliderComponent* collider)
{
	sf::FloatRect hitbox = collider->GetHitbox();

	float X1 = clamp(hitbox.position.x, positionRange.x, positionRange.y);
	float Y1 = clamp(hitbox.position.y, positionRange.x, positionRange.y);
	float X2 = clamp(hitbox.position.x + hitbox.size.x, positionRange.x, positionRange.y);
	float Y2 = clamp(hitbox.position.y + hitbox.size.y, positionRange.x, positionRange.y);

	std::pair<int, int> topLeftCell = GetCellFromPosition(X1, Y1);
	std::pair<int, int> bottomRightCell = GetCellFromPosition(X2, Y2);

	if (topLeftCell.first < 0 && bottomRightCell.first < 0)
		return;

	for (int x = topLeftCell.first; x <= bottomRightCell.first; x++)
	{
		for (int y = topLeftCell.second; y <= bottomRightCell.second; y++)
		{
			container->push_back(&grid[x][y]);
		}
	}
}

std::pair<int,int> ColliderGridCache::GetCellFromPosition(float x, float y)
{
	//position outside of the grid
	if (x < positionRange.x || x > positionRange.y || y < positionRange.x || y > positionRange.y)
		return std::pair<int, int>(-1,-1);

	//positionRange.x is the small bound
	//min in case position == big bound
	int lineIndex = std::min((int)floor((x - positionRange.x) / (float)cellSize), gridSize - 1);
	int columnIndex = std::min((int)floor((y - positionRange.x) / (float)cellSize), gridSize - 1);

	return std::pair<int, int>(lineIndex, columnIndex);
}

float ColliderGridCache::clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
