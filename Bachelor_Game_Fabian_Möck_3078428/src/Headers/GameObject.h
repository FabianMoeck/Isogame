#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <list>

#include "../../Collection.h"
#include <queue>

class GameObject
{
public:
	bool selectable;
	bool selected = false;
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 color;
    IUnit* u = nullptr;

    bool calculating = false;
    std::queue<GameObject*> buildingQueue;
    GameObject* current = nullptr;

    enum class GameObjectType
    {
        FootSoldier,
        Scout,
        LightTank,
        HeavyTank,
        MainBuilding,
        Barracks,
        Factory
    };
    GameObjectType type;
    static const char* type_tostring(int type) {
        const char* types[] = { "FootSoldier", "Scout", "LightTank", "HeavyTank", "MainBuilding", "Barracks", "Factory"};
        return types[type];
    };
    
    enum Team
    {
        Neutral,
        Player,
        Ally,
        Enemy
    };
    Team team;

	GameObject();
	GameObject(std::string _name, glm::vec3 _position, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable, GameObjectType _type, Team _team);
	~GameObject();
    friend bool operator==(const GameObject first, const GameObject second);
    friend bool operator!=(const GameObject first, const GameObject second);
    void teleportObject(glm::vec3** grid, const int _X, const int _Y);

    float* CubeVertices();
    unsigned int* Indices();
    int sizeOfVertices();
    int sizeOfIndices();

    static void removeObject(std::list<GameObject*> *s, IUnit* remove) {
        for (GameObject* g : *s) {
            if (g->u == remove) {
                s->remove(g);
                g->~GameObject();
                break;
            }
        }
    };

    static GameObject* getGameObjectFromUnit(std::list<GameObject*>* s, IUnit* retrieve) {
        for (GameObject* g : *s) {
            if (g->u == retrieve) {
                return g;
            }
        }
    };

private:
	float cubeVertices[108] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
    };
	unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    void initType(std::string _name, Team _team, int _type);
};

inline float* GameObject::CubeVertices()
{
    return cubeVertices;
}

inline unsigned int* GameObject::Indices()
{
    return indices;
}

inline int GameObject::sizeOfVertices()
{
    int size = sizeof(float) * 108;
    return size;
}

inline int GameObject::sizeOfIndices()
{
    int size = sizeof(unsigned int) * 6;
    return size;
}

#endif // !GAMEOBJECT_H

