#include "../Headers/GameObject.h"
#include <iostream>

bool operator==(const GameObject first, const GameObject second) {
    return (first.u->name == second.u->name);
}

bool operator!=(const GameObject first, const GameObject second) {
    return (first.u->name != second.u->name);
}

void GameObject::teleportObject(glm::vec3** grid, const int _X, const int _Y) {
    glm::vec3 newPos = glm::vec3(grid[_X][_Y].x, position.y, grid[_X][_Y].z);
    position = newPos;
}

GameObject::GameObject() {
    selectable = false;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    angle = 0.0f;
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    type = GameObjectType::FootSoldier;
    team = Team::Neutral;

    initType("New Unit", Team::Neutral, static_cast<int>(type));
}

GameObject::GameObject(std::string _name, glm::vec3 _position, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable, GameObjectType _type, Team _team)
{
    position = _position;
    scale = _scale;
    angle = _angle;
    color = _color;
    selectable = _selectable;
    type = _type;
    team = _team;

    initType(_name, _team, static_cast<int>(_type));
}

GameObject::~GameObject()
{
}

void GameObject::initType(std::string _name, Team _team, int _type) {
    switch (type) {
        //Units
    case GameObjectType::FootSoldier:
        u = new FootSoldier(_name, _team, _type); break;
    case GameObjectType::Scout:
        u = new Scout(_name, _team, _type); break;
    case GameObjectType::HeavyTank:
        u = new HeavyTank(_name, _team, _type); break;
    case GameObjectType::LightTank:
        u = new LightTank(_name, _team, _type); break;

        //Buildings
    case GameObjectType::MainBuilding:
        u = new MainBuilding(_name, _team, _type); break;
    case GameObjectType::Barracks:
        u = new Barracks(_name, _team, _type); break;
    case GameObjectType::Factory:
        u = new Factory(_name, _team, _type); break;
    case GameObjectType::Wall:
        u = new Wall(_name, _team, _type); break;
    default:
        break;
    }
}