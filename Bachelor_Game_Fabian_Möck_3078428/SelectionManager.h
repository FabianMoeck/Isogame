#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include <list>
#include "GameObject.h"

class SelectionManager
{
public:
	static SelectionManager* getInstance();
	std::list<GameObject> selection;

private:
	static SelectionManager* instance;
	SelectionManager();
};

#endif // !SELECTIONMANAGER_H
