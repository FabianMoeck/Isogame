#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"

class Cube : GameObject
{
public:
	Cube();
	Cube(glm::vec3 _pos);
	Cube(glm::vec3 _pos, glm::vec3 _color);
	Cube(glm::vec3 _pos, glm::vec3 _color, glm::vec3 _scale);
	Cube(glm::vec3 _pos, float _r, float _g, float _b);
	Cube(float _x, float _y, float _z);
	Cube(float _x, float _y, float _z, glm::vec3 _color);
	Cube(float _x, float _y, float _z, float _r, float _g, float _b);
	Cube(float _x, float _y, float _z, float _r, float _g, float _b, float _sX, float _sY, float _sZ);
	~Cube();

private:
	void scaleCube();
	void rotateCube(float angle);
};

#pragma region Constructors / Deconstructor
inline Cube::Cube()
{
}

inline Cube::Cube(glm::vec3 _pos)
{
	position = _pos;
}

inline Cube::Cube(glm::vec3 _pos, glm::vec3 _color)
{
	position = _pos;
	color = _color;
}

inline Cube::Cube(glm::vec3 _pos, glm::vec3 _color, glm::vec3 _scale)
{
	position = _pos;
	color = _color;
	scale = _scale;
}

inline Cube::Cube(glm::vec3 _pos, float _r, float _g, float _b)
{
	position = _pos;
	color = glm::vec3(_r, _g, _b);
}

inline Cube::Cube(float _x, float _y, float _z)
{
	position = glm::vec3(_x, _y, _z);
}

inline Cube::Cube(float _x, float _y, float _z, glm::vec3 _color)
{
	position = glm::vec3(_x, _y, _z);
	color = _color;
}

inline Cube::Cube(float _x, float _y, float _z, float _r, float _g, float _b)
{
	position = glm::vec3(_x, _y, _z);
	color = glm::vec3(_r, _g, _b);
}

inline Cube::Cube(float _x, float _y, float _z, float _r, float _g, float _b, float _sX, float _sY, float _sZ)
{
	position = glm::vec3(_x, _y, _z);
	color = glm::vec3(_r, _g, _b);
	scale = glm::vec3(_sX, _sY, _sZ);
}

inline Cube::~Cube()
{
}
#pragma endregion

inline void Cube::scaleCube()
{
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
}

inline void Cube::rotateCube(float angle)
{
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
}

#endif // !CUBE_H

