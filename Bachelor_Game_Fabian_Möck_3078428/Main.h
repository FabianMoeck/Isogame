#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <list>
#include <iostream>

#include "src/Headers/Shader.h"
#include "src/Headers/Scene.h"
#include "src/Headers/SelectionManager.h"
#include "src/Headers/Map.h"
#include "src/Headers/ghostGameObject.h"
#include "Pathfinding.h"
#include "PathRequest.h"

std::ostream& operator<<(std::ostream& os, const glm::vec3 vec);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//input
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//VAO
void createCubeVAO();
void createPlaneVAO();
void createGridVAO();

//GameObjects
void drawCube(const unsigned int shaderID, const GameObject* toDraw);
void drawCubePicking(const unsigned int shaderID, const GameObject* toDraw, const int NoOfObject);
void drawPlane(const unsigned int shaderID, const Map *map);
void drawPickingPlane(const unsigned int shaderID, const Map *map);
void drawGhostObject(unsigned int shaderID, GhostGO* todraw);
void ghostPosition(GLFWwindow* window, GhostGO* ghost, Map* map);

//color
glm::vec3 RGB(const float _R, const float _G, const float _B);                //convert any RGB value into a range from 0-1