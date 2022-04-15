#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <list>
#include <glm/glm.hpp>
#include "GameObject.h"

class SelectionManager
{
public:
	static SelectionManager* getInstance();
	std::list<GameObject> selection;
	glm::vec3 selectionColor;

	std::list<GameObject> savedSelections[10];

	std::list<GameObject> returnSelection(int index);
	void saveSelection(std::list<GameObject> currentSelection, int index);

private:
	static SelectionManager* instance;
	SelectionManager();

	
};

#endif // !SELECTIONMANAGER_H
