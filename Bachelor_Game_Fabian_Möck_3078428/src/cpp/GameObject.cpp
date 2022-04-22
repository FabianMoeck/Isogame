#include "../Headers/GameObject.h"

bool operator==(const GameObject first, const GameObject second) {
    return (first.name == second.name);
}

bool operator!=(const GameObject first, const GameObject second) {
    return (first.name != second.name);
}

void GameObject::move(const glm::vec3** grid, const int _X, const int _Y) {

}

GameObject::GameObject() {
    name = "New GameObject";
    selectable = false;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    angle = 0.0f;
    color = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(std::string _name, glm::vec3 _position, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable, GameObjectType _type)
{
    name = _name;
    position = _position;
    scale = _scale;
    angle = _angle;
    color = _color;
    selectable = _selectable;
    type = _type;
}

GameObject::~GameObject()
{
}