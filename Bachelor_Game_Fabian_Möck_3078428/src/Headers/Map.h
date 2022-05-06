#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>
//Wip
#include "GameObject.h"
#include <list>
#include "../../Node.h"

class Map
{
	
public:
	glm::vec2 mapSize;
	glm::vec3 mapColor;
	glm::vec3* mapPosition;

	Node*** grid;

	Map();
	Map(const int _x, const int _y, const glm::vec3 _mapColor, const int _gripMulti);
	Map(const glm::vec2 _mapSize, const glm::vec3 _mapColor, const int _gripMulti);
	~Map();

private:
	int gridMultiplier = 5;

	glm::vec3* initMap(const glm::vec2 mapSize);
	glm::vec3* initMap(const int mapSizeX, const int mapSizeY);

	Node*** initGrid(const int sizeX, const int sizeY);

	/*glm::vec2 mapToGrid(glm::vec3 mapCord);
	void blockedGrid(const std::list<GameObject> sceneObjects);*/
};

#endif // !MAP_H
