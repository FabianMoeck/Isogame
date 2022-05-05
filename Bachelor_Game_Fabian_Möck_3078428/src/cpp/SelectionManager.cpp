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

std::list<GameObject*> SelectionManager::returnSelection(const int index) {
	SelectionManager* s = SelectionManager::getInstance();
	return s->savedSelections[index];
}

void SelectionManager::saveSelection(const std::list<GameObject*> currentSelection, const int index) {
	SelectionManager* s = SelectionManager::getInstance();
	s->savedSelections[index] = currentSelection;
}

void SelectionManager::setSelection(const std::list<GameObject*> newSelection) {
	selection = newSelection;
}
