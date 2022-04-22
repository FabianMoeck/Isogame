#include "Map.h"

Map::Map() {

}

Map::Map(const int _x, const int _y, const glm::vec3 _mapColor, const int _gripMulti)
{
    mapSize = glm::vec2(_x, _y);
    mapColor = _mapColor;
    gridMultiplier = _gripMulti;
    mapPosition = initMap(_x, _y);
    grid = initGrid(_x, _y);
}

Map::Map(const glm::vec2 _mapSize, const glm::vec3 _mapColor, const int _gripMulti) {
    mapSize = _mapSize;
    mapColor = _mapColor;
    gridMultiplier = _gripMulti;
    mapPosition = initMap(_mapSize);
    grid = initGrid(mapSize.x, mapSize.y);
}

Map::~Map()
{
}

glm::vec3* Map::initMap(const glm::vec2 mapSize)
{
    glm::vec3* planePosition;
    planePosition = new glm::vec3[mapSize.x * mapSize.y];
    int count = 0;
    for (unsigned int i = 0; i < mapSize.x; i++)
    {
        for (unsigned int j = 0; j < mapSize.y; j++)
        {
            planePosition[count] = glm::vec3(0.0f + i, -0.5f, 0.0f + j);
            count++;
        }
    }
    return planePosition;
}

glm::vec3* Map::initMap(const int mapSizeX, const int mapSizeY)
{
    return initMap(glm::vec2(mapSizeX, mapSizeY));
}

glm::vec3** Map::initGrid(const int sizeX, const int sizeY) {
    glm::vec3** gridPosition = new glm::vec3 * [sizeX * gridMultiplier];
    for (int i = 0; i < sizeX * gridMultiplier; ++i)
        gridPosition[i] = new glm::vec3[sizeY * gridMultiplier];

    for (int i = 0; i < sizeX * gridMultiplier; i++)
    {
        for (int j = 0; j < sizeY * gridMultiplier; j++)
        {
            gridPosition[i][j] = glm::vec3(0.0f + i/ gridMultiplier, -0.5f, 0.0f + j/ gridMultiplier);
        }
    }

    return gridPosition;
}