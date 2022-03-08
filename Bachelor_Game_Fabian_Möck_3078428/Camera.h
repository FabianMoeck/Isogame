#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 cameraRight;
	glm::vec3 up;
	glm::vec3 cameraDirection;

	Camera();
	~Camera();

	void updateCameraVec3() {
		glm::vec3 camerPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(camerPos - cameraTarget);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	}

	void setProjection(unsigned int shaderID) {
		int projectloc = glGetUniformLocation(shaderID, "projection");
		glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void setView(unsigned int shaderID) {
		int viewloc = glGetUniformLocation(shaderID, "view");
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
	}

	void setModel(unsigned int shaderID) {
		int modelloc = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
	}

private:
	void projectionMatrix() {

	}

	void viewMatrix() {

	}
};

Camera::Camera()
{
}

Camera::~Camera()
{
}

#endif // !CAMERA_H

