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

	std::list<GameObject> returnSelection(const int index);
	void saveSelection(const std::list<GameObject> currentSelection, const int index);
	void setSelection(const std::list<GameObject> newSelection);

private:
	static SelectionManager* instance;
	SelectionManager();

	
};

#endif // !SELECTIONMANAGER_H
