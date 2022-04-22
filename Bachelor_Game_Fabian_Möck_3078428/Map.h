#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>

class Map
{
	
public:
	glm::vec2 mapSize;
	glm::vec3 mapColor;
	glm::vec3* mapPosition;

	glm::vec3** grid;

	Map();
	Map(const int _x, const int _y, const glm::vec3 _mapColor, const int _gripMulti);
	Map(const glm::vec2 _mapSize, const glm::vec3 _mapColor, const int _gripMulti);
	~Map();

private:
	int gridMultiplier = 5;

	glm::vec3* initMap(const glm::vec2 mapSize);
	glm::vec3* initMap(const int mapSizeX, const int mapSizeY);

	glm::vec3** initGrid(const int sizeX, const int sizeY);
};

#endif // !MAP_H
