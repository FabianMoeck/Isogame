#include "../Headers/Map.h"
#include <iostream>

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

Node*** Map::initGrid(const int sizeX, const int sizeY) {
    Node*** gridPosition = new Node **[sizeX * gridMultiplier];
    for (int i = 0; i < sizeX * gridMultiplier; ++i)
        gridPosition[i] = new Node*[sizeY * gridMultiplier];

    for (int i = 0; i < sizeX * gridMultiplier; i++)
    {
        for (int j = 0; j < sizeY * gridMultiplier; j++)
        {
            float x = i, y = j;
            /*if (i == 0 && j == 0) {
                gridPosition[i][j] = new Node(glm::vec3(0.0f + x / gridMultiplier, -0.5f, 0.0f + y / gridMultiplier), true, i, j);
            }
            else*/
                gridPosition[i][j] = new Node(glm::vec3(0.0f + x / gridMultiplier, -0.5f, 0.0f + y / gridMultiplier), true, i, j);
        }
    }

    return gridPosition;
}

void Map::resetGrid() {
    for (int i = 0; i < mapSize.x * gridMultiplier; i++)
    {
        for (int j = 0; j < mapSize.y * gridMultiplier; j++)
        {
            Node* n = grid[i][j];
            n->walkable = true;
        }
    }
}

void Map::updateGrid(const std::list<GameObject*> *scene) {
    resetGrid();
    for (GameObject* g : *scene) {
        if (g->position.x >= 0.0f && g->position.z >= 0.0f) {
            Node* startNode = nodeFromMap(g->position);
            if (g->scale.x >= 1 && g->scale.z >= 1) {
                for (int i = 0; i < g->scale.x * gridMultiplier; i++) {
                    for (int j = 0; j < g->scale.z * gridMultiplier; j++) {
                        Node* n = nodeFromMap(glm::vec3(startNode->position.x + ((1 / gridMultiplier) * i), 0.5f, startNode->position.z + ((1 / gridMultiplier) * j)));
                        n->walkable = false;
                    }
                }
            }
        }
    }
}

Node* Map::nodeFromMap(glm::vec3 mapCord) {
    float percentageX = mapCord.x / mapSize.x;               //convert position in percentage of the world
    float percentageY = mapCord.z / mapSize.y;

    percentageX = clip(percentageX, 0, 1);                             //clamp percentages inside the area
    percentageY = clip(percentageY, 0, 1);

    int x = (int)round(percentageX * mapSize.x * gridMultiplier);
    int y = (int)round(percentageY * mapSize.y * gridMultiplier);

    return grid[x][y];
}

std::list<Node*> Map::getNeighbours(Node* node) {
    std::list<Node*> neighbours;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)                   //run through a 3x3 block
        {
            if (x == 0 && y == 0)                       //center of that block (node you want the neighbours from)
                continue;                       //skip

            int checkX = node->gridX + x;
            int checkY = node->gridY + y;

            if (checkX >= 0 && checkX < mapSize.x * gridMultiplier && checkY >= 0 && checkY < mapSize.y * gridMultiplier)         //check if neighbour node is in the grid (not top, right, bottom, left)
            {
                neighbours.push_back(grid[checkX][checkY]);               //add this neighbour to the list
            }
        }
    }
    return neighbours;
}
