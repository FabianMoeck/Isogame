#include "../Headers/EnemyManager.h"

EnemyManager::EnemyManager() {

}

EnemyManager::EnemyManager(std::list<AttackRequest*>* _atk_list, std::list<BuildRequest*>* _build_list, std::list<PathRequest*>* _move_list, GameObject* _factory, GameObject* _barrack, Scene* _currentScene, Pathfinding* _pathfinder)
{
	currentScene = _currentScene;
	pathfinder = _pathfinder;

	atk_list = _atk_list;
	build_list = _build_list;
	move_list = _move_list;

	factory = _factory;
	barrack = _barrack;
}

EnemyManager::~EnemyManager(){

}

void EnemyManager::addMoney(float _deltaTime) {
	currentMoney += gatherAmount;
	timeSinceLastAdd = 0.0f;
	if (currentMoney > maxMoney) {
		startProduce();
	}
}

void EnemyManager::reduceMoney(int amount) {
	currentMoney -= amount;
}

void EnemyManager::startProduce() {
	if (currentUnits.size() < unitAttackCount) {
		BuildRequest* b = nullptr;
		GameObject::GameObjectType type = getRandomType();
		switch (type) {
		case GameObject::GameObjectType::FootSoldier:
			b = new BuildRequest(barrack, "Enemy_FS", glm::vec3(1), 0.0f, glm::vec3(184.0f/255.0f,40.0f/255.0f,55.0f/255.0f), false, type, GameObject::Team::Enemy); break;
		case GameObject::GameObjectType::Scout:
			b = new BuildRequest(barrack, "Enemy_S", glm::vec3(1), 0.0f, glm::vec3(184.0f/255.0f,40.0f/255.0f,55.0f/255.0f), false, type, GameObject::Team::Enemy); break;
		case GameObject::GameObjectType::HeavyTank:
			b = new BuildRequest(factory, "Enemy_HT", glm::vec3(1), 0.0f, glm::vec3(184.0f/255.0f,40.0f/255.0f,55.0f/255.0f), false, type, GameObject::Team::Enemy); break;
		case GameObject::GameObjectType::LightTank:
			b = new BuildRequest(factory, "Enemy_LT", glm::vec3(1), 0.0f, glm::vec3(184.0f/255.0f,40.0f/255.0f,55.0f/255.0f), false, type, GameObject::Team::Enemy); break;
		default:
			break;
		}
		if (b != nullptr) {
			currentUnits.push_back(b->getData());
			build_list->push_back(b);
			reduceMoney(b->parent->u->cost);
		}
	}
	else
		attackPlayer();
}

void EnemyManager::attackPlayer() {
	for (GameObject* g : currentUnits) {
		GameObject* target = getRandomPlayerUnit();
		ITroop *t = (ITroop*)g->u;
		if (glm::distance(g->position, target->position) <= t->range) {
			AttackRequest* a = new AttackRequest((ITroop*)g->u, target->u);
			atk_list->push_back(a);
		}
		else
		{
			PathRequest *p = new PathRequest(g, target, *pathfinder, atk_list);
			move_list->push_back(p);
		}
	}
	currentUnits.clear();
}

GameObject::GameObjectType EnemyManager::getRandomType() {
	int r = std::rand() % 4;
	GameObject::GameObjectType t;
	switch (r) {
	case 0:
		t = GameObject::GameObjectType::FootSoldier; break;
	case 1:
		t = GameObject::GameObjectType::Scout; break;
	case 2:
		t = GameObject::GameObjectType::HeavyTank; break;
	case 3:
		t = GameObject::GameObjectType::LightTank; break;
	default:
		t = GameObject::GameObjectType::FootSoldier; break;
	}
	return t;
}

GameObject* EnemyManager::getRandomPlayerUnit() {
	GameObject* g = nullptr;
	while (true) {
		int r = std::rand() % currentScene->SceneList.size();
		g = get(&currentScene->SceneList, r);
		if (g->team == 1) {
			break;
		}
	}

	if (g != nullptr)
		return g;
	else
		exit(1);
}