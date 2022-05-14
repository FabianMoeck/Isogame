#include "Pathfinding.h"

Pathfinding::Pathfinding(Map *_map)
{
	map = _map;
}

Pathfinding::~Pathfinding()
{
}

std::list<glm::vec3*> Pathfinding::findPath(glm::vec3 startPos, glm::vec3 targetPos) {

	std::list<glm::vec3*>waypoints;
	bool pathSuccess = false;

	Node* startN = map->nodeFromMap(startPos);
	Node* endN = map->nodeFromMap(targetPos);

	if (endN->walkable) {
		std::unordered_set<Node*> openSet = std::unordered_set<Node*>();
		std::unordered_set<Node*> closedSet = std::unordered_set<Node*>();
		openSet.insert(startN);

		while (openSet.size() > 0) {
			Node* currentNode = *openSet.begin();
			openSet.erase(currentNode);
			closedSet.insert(currentNode);

			if (currentNode->position == endN->position) {
				pathSuccess = true;
				break;
			}

			for (Node* neighbour : map->getNeighbours(currentNode)) {
				std::unordered_set<Node*>::iterator itClosed = std::find(closedSet.begin(), closedSet.end(), neighbour);

				if (!neighbour->walkable || itClosed != closedSet.end()) {
					continue;
				}

				int newMoveCostToNeighbour = currentNode->gCost + getDistance(currentNode, neighbour);

				std::unordered_set<Node*>::iterator itOpen = std::find(openSet.begin(), openSet.end(), neighbour);

				if (newMoveCostToNeighbour < neighbour->gCost || itOpen == openSet.end()) {
					neighbour->gCost = newMoveCostToNeighbour;
					neighbour->hCost = getDistance(neighbour, endN);
					neighbour->parent = currentNode;

					if (itOpen == openSet.end()) {
						openSet.insert(neighbour);
					}
				}
			}
		}
	}

	if (pathSuccess) {
		waypoints = retracePath(startN, endN);
	}
	return waypoints;
}

std::list<glm::vec3*> Pathfinding::retracePath(Node* startNode, Node* targetNode) {
	std::list<Node*> path;
	Node* currentNode = targetNode;                     //start from end and move back thorugh the parents

	while (*currentNode != *startNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->parent;
	}

	std::list<glm::vec3*> waypoints = simplifiePath(&path);          //only add nodes where the dircetion changes
	waypoints.reverse();

	return waypoints;
}

std::list<glm::vec3*> Pathfinding::simplifiePath(std::list<Node*> *path) {
	std::list<glm::vec3*> waypoints;
	glm::vec2 directionOld = glm::vec2(0.0f);

	for (int i = 1; i < path->size(); i++)
	{
		glm::vec2 directionNew = glm::vec2(get(*path, i - 1)->gridX - get(*path, i)->gridX, get(*path, i - 1)->gridY - get(*path, i)->gridY);       //calc the direction of the vector between 2 nodes in the path
		if (directionNew != directionOld)
		{
			waypoints.push_back(&get(*path, i)->position);                //add this node to the simpliefied list
		}
		directionOld = directionNew;                        //update the old direction
	}

	return waypoints;
}

int Pathfinding::getDistance(Node* a, Node* b) {
	int distX = std::abs(a->gridX - b->gridX);
	int distY = std::abs(a->gridY - b->gridY);

	if (distX > distY)
	{                                                       //14 = diagonal 10*1.4      10 = 10*1 straight (Costto move to a node) 
		return 14 * distY + 10 * (distX - distY);               //14 * the short distance + 10 * the long distance
	}
	else
		return 14 * distX + 10 * (distY - distX);
}