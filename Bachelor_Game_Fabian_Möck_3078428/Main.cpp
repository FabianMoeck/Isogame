#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"

#define SCREENWIDTH 800
#define SCREENHIGHT 600

#define MAPSIZE_X 10
#define MAPSIZE_Y 10

//overloading << for vec3
std::ostream& operator<<(std::ostream& os, const glm::vec3 vec);
std::ostream& operator<<(std::ostream& os, const glm::vec3 vec)
{
    os << vec.x << "/" << vec.y << "/" << vec.z;
    return os;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//input
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//color
glm::vec3 RGB(float _R, float _G, float _B);                //convert any RGB value into a range from 0-1

//VAO
void createCubeVAO();
void createPlaneVAO();
unsigned int CUBE_VAO;
unsigned int PLANE_VAO;

glm::vec3* initMap(glm::vec2 mapSize);
glm::vec3* initMap(int mapSizeX, int mapSizeY);

glm::vec3 cameraPos = glm::vec3(15.0f, 10.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

enum class Direction {forward, back, right, left};
void moveCamera(Direction direction);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float fov = 45.0f;

int main()
{
    glfwInit();                                                 //Init GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);               //set Version of GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHIGHT, "ISO Game", NULL, NULL);        //create Window (width, height, name, monitor, share)

    if (window == NULL) {                                       //check if window was created
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                         //make window the current context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {          //check if GLAD was correctly implemented
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREENWIDTH, SCREENHIGHT);                                     //set size for window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);          //set a call for a method if window gets resized

    //mouseCallback
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //setup shaders
    Shader shader = Shader();           //init Shader (create Shader objects and bind them)

    glEnable(GL_DEPTH_TEST);

    //cube
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 cubeColors[] = {
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 1.0f, 1.0f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(0.2f, 0.7f, 0.5f)
    };

    //plane
    glm::vec2 mapSize = glm::vec2(MAPSIZE_X, MAPSIZE_Y);
    glm::vec3* planePosition = initMap(mapSize);

    glm::vec3 planeColor = RGB(17.0f, 138.0f, 19.0f);

    //vertex input into buffer
    createCubeVAO();
    createPlaneVAO();

    //render loop
    while (!glfwWindowShouldClose(window)) {                    //keep window running till it gets told to close
        //input
        processInput(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        //rendering
        glm::vec3 bgColor = RGB(68, 135, 201);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //use shader
        shader.use();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 direction = glm::vec3(-0.637695f, -0.707106f, -0.305524f);
        cameraFront = glm::normalize(direction);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), static_cast<float>(SCREENWIDTH) / static_cast<float>(SCREENHIGHT), 0.1f, 100.0f);

        glBindVertexArray(CUBE_VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

            int modelloc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

            //set color for cubes
            float r = cubeColors[i].x;
            float g = cubeColors[i].y;
            float b = cubeColors[i].z;
            int vertexcolor = glGetUniformLocation(shader.ID, "color");
            glUniform3f(vertexcolor, r,g,b);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(PLANE_VAO);
        for (unsigned int i = 0; i < mapSize.x * mapSize.y; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, planePosition[i]);
            float angle = 90;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

            int modelloc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

            float r = planeColor.x;
            float g = planeColor.y;
            float b = planeColor.z;
            int vertexcolor = glGetUniformLocation(shader.ID, "color");
            glUniform3f(vertexcolor, r, g, b);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        //camera
        glm::vec3 camerPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(camerPos - cameraTarget);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        int projectloc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
        int viewloc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

        //check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();                                       //check for any event happening in the window, e.g input
    }


    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteVertexArrays(1, &PLANE_VAO);
    shader.deleteShader();                      //delete Shader

    glfwTerminate();                                    //disconnect GLFW
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);                    //set new Values for viewport
}

#pragma region Input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveCamera(Direction::forward);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveCamera(Direction::back);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveCamera(Direction::left);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveCamera(Direction::right);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double percentage = 0.09;
    double up = 0, down = 0, left = 0, right = 0;
    down = SCREENHIGHT - (SCREENHIGHT * percentage);
    up = 0 + (SCREENHIGHT * percentage);
    left = 0 + (SCREENWIDTH * percentage);
    right = SCREENWIDTH - (SCREENWIDTH * percentage);
    
    double x = xpos, y = ypos;
    while (y < up) {
        moveCamera(Direction::forward);
        glfwGetCursorPos(window, &x, &y);
    }
    while (y > down) {
        moveCamera(Direction::back);
        glfwGetCursorPos(window, &x, &y);
    }
    while (x > right) {
        moveCamera(Direction::right);
        glfwGetCursorPos(window, &x, &y);
    }
    while (x < left) {
        moveCamera(Direction::left);
        glfwGetCursorPos(window, &x, &y);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 10.0f)
        fov = 10.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
#pragma endregion

glm::vec3 RGB(float _R, float _G, float _B) {
    float r = _R / 255;
    float g = _G / 255;
    float b = _B / 255;

    return glm::vec3(r, g, b);
}

void moveCamera(Direction direction) {
    float cameraSpeed = 0.01f * deltaTime;                   //adjust accordingly
    glm::vec3 xzPlane = glm::vec3(1.0f, 0.0f, 1.0f);              //vector on xz plane
    if (direction == Direction::forward)
        cameraPos += cameraSpeed * xzPlane * cameraFront;
    if (direction == Direction::back)
        cameraPos -= cameraSpeed * xzPlane * cameraFront;
    if (direction == Direction::left)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (direction == Direction::right)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void createCubeVAO()
{
    //vertices for each cube (each side)
    float cubeVertices[] = {
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

    unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &CUBE_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(CUBE_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

void createPlaneVAO()
{
    float planeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    };

    unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &PLANE_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(PLANE_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

#pragma region Map
glm::vec3* initMap(glm::vec2 mapSize)
{
    glm::vec3* planePosition;
    planePosition = new glm::vec3[mapSize.x * mapSize.y];
    int count = 0;
    for (unsigned int i = 0; i < mapSize.x; i++)
    {
        for (unsigned int j = 0; j < mapSize.y; j++)
        {
            planePosition[count] = glm::vec3(0.0f + i, -0.5f, 0.0f + j);
            count++;
        }
    }
    return planePosition;
}

glm::vec3* initMap(int mapSizeX, int mapSizeY)
{
    return initMap(glm::vec2(mapSizeX, mapSizeY));
}
#pragma endregion


