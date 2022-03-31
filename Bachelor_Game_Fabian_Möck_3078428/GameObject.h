#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class GameObject
{
public:
	std::string name;
	bool selectable;
	bool selected = false;
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 color;

	GameObject();
	GameObject(std::string _name, glm::vec3 _position, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable);
	~GameObject();

    float* CubeVertices();
    unsigned int* Indices();
    int sizeOfVertices();
    int sizeOfIndices();

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
};

GameObject::GameObject() {
    name = "New GameObject";
    selectable = false;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    angle = 0.0f;
    color = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(std::string _name, glm::vec3 _position, glm::vec3 _scale, float _angle, glm::vec3 _color, bool _selectable)
{
    name = _name;
    position = _position;
    scale = _scale;
    angle = _angle;
    color = _color;
    selectable = _selectable;
}

GameObject::~GameObject()
{
}

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

