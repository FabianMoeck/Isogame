#include "../Headers/SelectionManager.h"

SelectionManager* SelectionManager::instance = 0;

SelectionManager::SelectionManager() {}

SelectionManager* SelectionManager::getInstance()
{
	if (instance == 0) {
		instance = new SelectionManager();
	}
	return instance;
}

std::list<GameObject> SelectionManager::returnSelection(int index) {
	SelectionManager* s = SelectionManager::getInstance();
	return s->savedSelections[index];
}

void SelectionManager::saveSelection(std::list<GameObject> currentSelection, int index) {
	SelectionManager* s = SelectionManager::getInstance();
	s->savedSelections[index] = currentSelection;
}
