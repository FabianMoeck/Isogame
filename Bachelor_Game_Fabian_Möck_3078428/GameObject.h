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
    friend bool operator==(const GameObject first, const GameObject second);
    friend bool operator!=(const GameObject first, const GameObject second);
    void move(glm::vec3 moveCom);
    void move(float x, float y);

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

