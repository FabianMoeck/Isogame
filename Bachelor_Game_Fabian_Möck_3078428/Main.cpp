#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <iostream>

#include "Shader.h"
#include "Scene.h"
#include "SelectionManager.h"

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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//color
glm::vec3 RGB(float _R, float _G, float _B);                //convert any RGB value into a range from 0-1

//VAO
void createCubeVAO();
void createPlaneVAO();
unsigned int CUBE_VAO;
unsigned int PLANE_VAO;

unsigned int shaderID;

//draw Objects / Scene
Scene scene_1;
void drawCube(unsigned int shaderID, GameObject toDraw);
void drawCubePicking(unsigned int shaderID, GameObject toDraw, int NoOfObject);
void drawPlane(unsigned int shaderID, glm::vec2 mapSize, glm::vec3 planeColor, glm::vec3* planePosition);

//Selection
SelectionManager* selManager;
double xPress = 0, yPress = 0;

//map
glm::vec3* initMap(glm::vec2 mapSize);
glm::vec3* initMap(int mapSizeX, int mapSizeY);

//camera
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(15.0f, 10.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool moveCam = false;
double screenPercentage = 0.09;
double screenUp = 0 + (SCREENHIGHT * screenPercentage), screenDown = SCREENHIGHT - (SCREENHIGHT * screenPercentage);
double screenLeft = 0 + (SCREENWIDTH * screenPercentage), screenRight = SCREENWIDTH - (SCREENWIDTH * screenPercentage);

enum class Direction {forward, back, right, left};
void moveCamera(Direction direction);
glm::vec3 screenToWorld(float screenX, float screenY);

float fov = 45.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //setup shaders
    Shader shader = Shader();           //init Shader (create Shader objects and bind them)
    shaderID = shader.ID;

    glEnable(GL_DEPTH_TEST);

    //init Scene and GO's
    scene_1 = Scene();
    GameObject cube1 = GameObject("testCube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 0.5f, 0.5f), true);
    GameObject cube2 = GameObject("Cube_White", glm::vec3(-2.0f, -0.5f, 3.0f), glm::vec3(2.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), false);
    GameObject cube3 = GameObject("Cube_Blue", glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 30.0f, RGB(50, 78, 168), true);
    scene_1.SceneList.push_back(cube1);
    scene_1.SceneList.push_back(cube2);
    scene_1.SceneList.push_back(cube3);

    selManager = SelectionManager::getInstance();

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

        glm::vec3 direction = glm::vec3(-0.637695f, -0.707106f, -0.305524f);        //direction the camera is looking
        cameraFront = glm::normalize(direction);

        projection = glm::perspective(glm::radians(fov), static_cast<float>(SCREENWIDTH) / static_cast<float>(SCREENHIGHT), 0.1f, 100.0f);

        //render Objects
        scene_1.active = true;              //set scene as active
        if (scene_1.active) {
            for (GameObject g : scene_1.SceneList)
            {
                drawCube(shader.ID, g);
            }
        }

        drawPlane(shader.ID, mapSize, planeColor, planePosition);

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

        //moveCamera with Mouseposition
        if(moveCam) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            if (y < screenUp)
                moveCamera(Direction::forward);
            if (y > screenDown)
                moveCamera(Direction::back);
            if (x > screenRight)
                moveCamera(Direction::right);
            if (x < screenLeft)
                moveCamera(Direction::left);
        }

        //check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();                                       //check for any event happening in the window, e.g input
    }

    glDeleteVertexArrays(1, &CUBE_VAO);
    glDeleteVertexArrays(1, &PLANE_VAO);
    shader.deleteShader();                      //delete Shader

    delete(selManager);

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
    if (ypos < screenUp || ypos > screenDown || xpos > screenRight || xpos < screenLeft)
        moveCam = true;
    else
        moveCam = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double pixelX = 1, pixelY = 1;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderID);
        
        int projectloc = glGetUniformLocation(shaderID, "projection");
        glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
        int viewloc = glGetUniformLocation(shaderID, "view");
        glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

        int objects = 0;
        for (GameObject Go : scene_1.SceneList)
        {
            if(Go.selectable)
                drawCubePicking(shaderID, Go, objects);
            objects++;
        }

        glFlush();
        glFinish();

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        double xRelease, yRelease;
        glfwGetCursorPos(window, &xRelease, &yRelease);
        yRelease = SCREENHIGHT - yRelease;

        //std::cout << "Cursor Pos start: " << xPress << "/" << yPress << std::endl;
        //std::cout << "Cursor Pos end: " << xRelease << "/" << yRelease << std::endl;

        //Problem 1: screenToWorld not right output
        /*GameObject newCube = GameObject("added", screenToWorld(xRelease, yRelease), glm::vec3(1.0f), 0.0f, glm::vec3(0.0f), false);
        scene_1.SceneList.push_back(newCube);*/

        if (xPress != xRelease || yPress != yRelease) {
            pixelX = xRelease - xPress;
            pixelY = yRelease - yPress;
            pixelX = glm::abs(pixelX);
            pixelY = glm::abs(pixelY);
        }

        unsigned char* data = new unsigned char[pixelX * pixelY * 4];

        glReadPixels(xRelease, yRelease, pixelX, pixelY, GL_RGBA, GL_UNSIGNED_BYTE, data);          //where start (lower left x+y), size 1,1 = one pixel, type, where to save

        std::list<int> pickedIDs = std::list<int>();

        for (int i = 0; i < pixelX * pixelY * 4; i += 4) {
            int pickedID = data[i] + data[i + 1] * 256 + data[i + 2] * 256 * 256;
            pickedIDs.push_back(pickedID);
            pickedIDs.sort();
            pickedIDs.unique();
        }

        if (pickedIDs.size() == 1 && pickedIDs.front() == 0x00ffffff) {                 //if only Background was selected
            std::cout << "Background -- Selection Cleared" << std::endl;
            selManager->selection.clear();
        }

        for (int ID : pickedIDs) {
            if (ID == 0x00ffffff) {
                continue;       //skip over background ID
            }
            else {
                if (scene_1.active) {
                    GameObject picked = scene_1.getGameObject(scene_1.SceneList, ID);
                    std::cout << "Picked Object: " << picked.name << std::endl;

                    selManager->selection.push_back(picked);            //add picked GameObject to selection
                    selManager->selection.unique();                 //remove any GameObject twice in the list
                }
            }
        }
        
        glfwSwapBuffers(window);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xPress, &yPress);
        yPress = SCREENHIGHT - yPress;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        if (selManager->selection.size() > 0) {
            for (GameObject selected : selManager->selection) {
                std::cout << selected.name << " moves" << std::endl;
            }
        }
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

void moveCamera(Direction direction) {
    float cameraSpeed = 5.0f * deltaTime;                   //adjust accordingly
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

glm::vec3 screenToWorld(float screenX, float screenY) {
    std::cout << "X/Y: " << screenX << "/" << screenY << std::endl;
    glm::vec4 screenPos = glm::vec4((screenX / SCREENWIDTH), 1.0f , (screenY / SCREENHIGHT), 1.0f);                  //create vec4 with values between 1 - 0
    std::cout <<"ScreenPos: "<< screenPos << std::endl;

    glm::mat4 projectView = projection * view;
    glm::mat4 invProjectView = glm::inverse(projectView);

    glm::vec4 pos = invProjectView * screenPos;
    //pos.y = 0.0f;
    std::cout << "WorldPos: " << pos << std::endl;
    return (glm::vec3)pos;
}

#pragma region Util
glm::vec3 RGB(float _R, float _G, float _B) {
    float r = _R / 255;
    float g = _G / 255;
    float b = _B / 255;

    return glm::vec3(r, g, b);
}
#pragma endregion

#pragma region Visualization
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

void drawCube(unsigned int shaderID, GameObject toDraw) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw.position);
    model = glm::rotate(model, glm::radians(toDraw.angle), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, toDraw.scale);

    int modelloc = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

    //set color for cubes
    float r = toDraw.color.x;
    float g = toDraw.color.y;
    float b = toDraw.color.z;
    int vertexcolor = glGetUniformLocation(shaderID, "color");
    glUniform3f(vertexcolor, r, g, b);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawCubePicking(unsigned int shaderID, GameObject toDraw, int nr) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw.position);
    model = glm::rotate(model, glm::radians(toDraw.angle), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, toDraw.scale);

    int modelloc = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

    //set color for cubes
    int r = (nr & 0x000000FF) >> 0;
    int g = (nr & 0x0000FF00) >> 8;
    int b = (nr & 0x00FF0000) >> 16;

    int vertexcolor = glGetUniformLocation(shaderID, "color");
    glUniform3f(vertexcolor, r / 255.0f, g / 255.0f, b / 255.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawPlane(unsigned int shaderID, glm::vec2 mapSize, glm::vec3 planeColor, glm::vec3* planePosition) {
    glBindVertexArray(PLANE_VAO);
    for (unsigned int i = 0; i < mapSize.x * mapSize.y; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, planePosition[i]);
        float angle = 90;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        int modelloc = glGetUniformLocation(shaderID, "model");
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

        float r = planeColor.x;
        float g = planeColor.y;
        float b = planeColor.z;
        int vertexcolor = glGetUniformLocation(shaderID, "color");
        glUniform3f(vertexcolor, r, g, b);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
#pragma endregion

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


