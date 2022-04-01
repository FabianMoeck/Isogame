#include "SelectionManager.h"

SelectionManager* SelectionManager::instance = 0;

SelectionManager::SelectionManager() {}

SelectionManager* SelectionManager::getInstance()
{
	if (instance == 0) {
		instance = new SelectionManager();
	}
	return instance;
}
