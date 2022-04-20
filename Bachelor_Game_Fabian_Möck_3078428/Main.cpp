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

#define INIT_SCREENWIDTH 800                //only initial screensize
#define INIT_SCREENHEIGTH 600

#define MAPSIZE_X 10
#define MAPSIZE_Y 10

glm::vec3 screenToWorld();

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//color
glm::vec3 RGB(const float _R, const float _G, const float _B);                //convert any RGB value into a range from 0-1

//Screen
int screenHeight = INIT_SCREENHEIGTH;
int screenWidth = INIT_SCREENWIDTH;

//VAO
void createCubeVAO();
void createPlaneVAO();
unsigned int CUBE_VAO;
unsigned int PLANE_VAO;

unsigned int shaderID;

//draw Objects / Scene
Scene scene_1;
void drawCube(const unsigned int shaderID, const GameObject toDraw);
void drawCubePicking(const unsigned int shaderID, const GameObject toDraw, const int NoOfObject);
void drawPlane(const unsigned int shaderID, const glm::vec2 mapSize, const glm::vec3 planeColor, const glm::vec3* planePosition);

//Selection
SelectionManager* selManager;
double xPress = 0, yPress = 0;

//map
glm::vec3* initMap(const glm::vec2 mapSize);
glm::vec3* initMap(const int mapSizeX, const int mapSizeY);

//camera
float fov = 45.0f;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(15.0f, 10.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool moveCam = false;
double screenPercentage = 0.09;
double screenUp = 0 + (screenHeight * screenPercentage), screenDown = screenHeight - (screenHeight * screenPercentage);
double screenLeft = 0 + (screenWidth * screenPercentage), screenRight = screenWidth - (screenWidth * screenPercentage);

enum class Direction {forward, back, right, left};
void moveCamera(Direction direction);

//time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//UI
double UIpercentageRight = 0.25;
double UIright = screenWidth - (screenWidth * UIpercentageRight);
double UIpercentageBottom = 0.15;
double UIbottom = screenHeight - (screenHeight * UIpercentageBottom);

int main()
{
    glfwInit();                                                 //Init GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);               //set Version of GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "ISO Game", NULL, NULL);        //create Window (width, height, name, monitor, share)

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

    glViewport(0, 0, screenWidth, screenHeight);                                     //set size for window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);          //set a call for a method if window gets resized

    //mouseCallback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //keyboard callback
    glfwSetKeyCallback(window, key_callback);

    //setup shaders
    Shader shader = Shader();           //init Shader (create Shader objects and bind them)
    shaderID = shader.ID;

    glEnable(GL_DEPTH_TEST);

#pragma region Scene/GameObjects
    //init Scene and GO's
    scene_1 = Scene();
    GameObject cube1 = GameObject("testCube", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 0.5f, 0.5f), true, GameObject::GameObjectType::Unit_1);
    GameObject cube2 = GameObject("Cube_White", glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), false, GameObject::GameObjectType::Building);
    GameObject cube3 = GameObject("Cube_Blue", glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 30.0f, RGB(50, 78, 168), true, GameObject::GameObjectType::Unit_1);
    GameObject cube4 = GameObject("Cube_Green", glm::vec3(-3.0f, 0.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 30.0f, RGB(50, 200, 10), true, GameObject::GameObjectType::Unit_2);
    scene_1.SceneList.push_back(cube1);
    scene_1.SceneList.push_back(cube2);
    scene_1.SceneList.push_back(cube3);
    scene_1.SceneList.push_back(cube4);

    selManager = SelectionManager::getInstance();
    selManager->selectionColor = RGB(240, 43, 69);                  //set Color that all selections are displayed in (current: Red)

    //plane
    glm::vec2 mapSize = glm::vec2(MAPSIZE_X, MAPSIZE_Y);
    glm::vec3* planePosition = initMap(mapSize);

    glm::vec3 planeColor = RGB(17.0f, 138.0f, 19.0f);

    //vertex input into buffer
    createCubeVAO();
    createPlaneVAO();
#pragma endregion

#pragma region ImGuI
    //ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //ImGui style
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowMenuButtonPosition = ImGuiDir_None;
#pragma endregion    

    //render loop
    while (!glfwWindowShouldClose(window)) {                    //keep window running till it gets told to close
        //input
        processInput(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        //rendering
        glm::vec3 bgColor = RGB(68, 135, 201);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //use shader
        shader.use();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::vec3 direction = glm::vec3(-0.637695f, -0.707106f, -0.305524f);        //direction the camera is looking
        cameraFront = glm::normalize(direction);

        projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

        //render Objects
        scene_1.active = true;              //set scene as active
        if (scene_1.active) {
            for (GameObject g : scene_1.SceneList)
            {
                drawCube(shader.ID, g);
            }
        }

        drawPlane(shader.ID, mapSize, planeColor, planePosition);

#pragma region UI content
        //ImGui UIRight
        ImGui::Begin("Test UI");
        ImGui::SetWindowPos(ImVec2(UIright, 0.5));
        ImGui::SetWindowSize(ImVec2((screenWidth - UIright), screenHeight));

        if (ImGui::BeginTabBar("tabs")) {
            if (ImGui::BeginTabItem("Buildings")) {
                if (ImGui::Button("Building 1", ImVec2((screenWidth - UIright) * 0.45, screenHeight * 0.1))) {
                    std::cout << "Building 1" << std::endl;
                }
                ImGui::SameLine();
                if (ImGui::Button("Building 2", ImVec2((screenWidth - UIright) * 0.45, screenHeight * 0.1))) {
                    std::cout << "Building 2" << std::endl;
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Units")) {
                if (ImGui::Button("Unit 1", ImVec2((screenWidth - UIright) * 0.45, screenHeight * 0.1))) {
                    std::cout << "Unit 1" << std::endl;
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        //BottomUI
        if (selManager->selection.size() > 0) {
            ImGui::Begin("BottomUI");
            ImGui::SetWindowPos(ImVec2(0, UIbottom));
            ImGui::SetWindowSize(ImVec2((screenWidth - (screenWidth * UIpercentageRight)), (screenHeight - UIbottom)));

            std::list<int> checkedtypes;
            std::list<GameObject> newSel;
            for (GameObject selected : selManager->selection) {
                ImGui::SameLine();
                if (checkedtypes.size() == 0) {
                    checkedtypes.push_back((int)selected.type);
                    if (ImGui::Button(selected.type_tostring((int)selected.type), ImVec2((screenWidth - UIright) * 0.35, (screenHeight - UIbottom) * 0.35))) {
                        for (GameObject withType : selManager->selection) {
                            if (withType.type == selected.type) {
                                newSel.push_back(withType);
                            }
                        }

                        selManager->selection = newSel;
                        break;
                    }
                }
                for (int i : checkedtypes) {
                    if (i != (int)selected.type) {
                        checkedtypes.push_back((int)selected.type);
                        if (ImGui::Button(selected.type_tostring((int)selected.type), ImVec2((screenWidth - UIright) * 0.35, (screenHeight - UIbottom) * 0.35))) {
                            for (GameObject withType : selManager->selection) {
                                if (withType.type == selected.type) {
                                    newSel.push_back(withType);
                                }
                            }
                            selManager->selection = newSel;
                            break;
                        }
                    }
                }
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion
        
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

    //end ImGui
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

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

    screenHeight = height;
    screenWidth = width;

    UIright = screenWidth - (screenWidth * UIpercentageRight);           //recalculate percentage values at borders
    UIbottom = screenHeight - (screenHeight * UIpercentageBottom);

    screenUp = 0 + (screenHeight * screenPercentage); 
    screenDown = screenHeight - (screenHeight * screenPercentage);
    screenLeft = 0 + (screenWidth * screenPercentage); 
    screenRight = screenWidth - (screenWidth * screenPercentage);
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

bool ctrl_Pressed = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //save Selection
    if (key == GLFW_KEY_LEFT_CONTROL) ctrl_Pressed = action == GLFW_PRESS;
    if (ctrl_Pressed) {
        switch (key) {
        case GLFW_KEY_0:
            selManager->saveSelection(selManager->selection, 0); break;
        case GLFW_KEY_1:
            selManager->saveSelection(selManager->selection, 1); break;
        case GLFW_KEY_2:
            selManager->saveSelection(selManager->selection, 2); break;
        case GLFW_KEY_3:
            selManager->saveSelection(selManager->selection, 3); break;
        case GLFW_KEY_4:
            selManager->saveSelection(selManager->selection, 4); break;
        case GLFW_KEY_5:
            selManager->saveSelection(selManager->selection, 5); break;
        case GLFW_KEY_6:
            selManager->saveSelection(selManager->selection, 6); break;
        case GLFW_KEY_7:
            selManager->saveSelection(selManager->selection, 7); break;
        case GLFW_KEY_8:
            selManager->saveSelection(selManager->selection, 8); break;
        case GLFW_KEY_9:
            selManager->saveSelection(selManager->selection, 9); break;
        default:
            break;
        }
    }

    //get saved Selection
    if (!ctrl_Pressed) {
        switch (key) {
        case GLFW_KEY_0:
            selManager->selection = selManager->returnSelection(0); break;
        case GLFW_KEY_1:
            selManager->selection = selManager->returnSelection(1); break;
        case GLFW_KEY_2:
            selManager->selection = selManager->returnSelection(2); break;
        case GLFW_KEY_3:
            selManager->selection = selManager->returnSelection(3); break;
        case GLFW_KEY_4:
            selManager->selection = selManager->returnSelection(4); break;
        case GLFW_KEY_5:
            selManager->selection = selManager->returnSelection(5); break;
        case GLFW_KEY_6:
            selManager->selection = selManager->returnSelection(6); break;
        case GLFW_KEY_7:
            selManager->selection = selManager->returnSelection(7); break;
        case GLFW_KEY_8:
            selManager->selection = selManager->returnSelection(8); break;
        case GLFW_KEY_9:
            selManager->selection = selManager->returnSelection(9); break;
        default:
            break;
        }
    }
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
        yRelease = screenHeight - yRelease;

        //GameObject newC = GameObject("new", screen_coords_to_world_coords(xRelease, SCREENHIGHT - yRelease, SCREENWIDTH, SCREENHIGHT), glm::vec3(1.0f), 0.0f, glm::vec3(1.0f), false);
        //scene_1.SceneList.push_back(newC);

        //calculated bottom left of square
        double centerX = 0, centerY = 0;
        double bottomLeftX = 0, bottomLeftY = 0;
        if (xPress != xRelease || yPress != yRelease) {
            pixelX = xRelease - xPress;
            pixelY = yRelease - yPress;
            pixelX = glm::abs(pixelX);          //size of square
            pixelY = glm::abs(pixelY);

            if (xPress > xRelease)                  //calculate center
                centerX = xPress - (pixelX / 2);
            else if (xPress < xRelease)
                centerX = xRelease - (pixelX / 2);
            if (yPress > yRelease)
                centerY = yPress - (pixelY / 2);
            else if (yPress < yRelease)
                centerY = yRelease - (pixelY / 2);

            bottomLeftX = centerX - (pixelX / 2);       //calc bottom left
            bottomLeftY = centerY - (pixelY / 2);
        }
        else {
            bottomLeftX = xPress;           //if only clicked (no multi-selection)
            bottomLeftY = yPress;
        }

        unsigned char* data = new unsigned char[pixelX * pixelY * 4];

        if (yRelease > screenHeight * UIpercentageBottom) {
            selManager->selection.clear();
        }

        glReadPixels(bottomLeftX, bottomLeftY, pixelX, pixelY, GL_RGBA, GL_UNSIGNED_BYTE, data);          //where start (lower left x+y), size 1,1 = one pixel, type, where to save

        std::list<int> pickedIDs = std::list<int>();

        for (int i = 0; i < pixelX * pixelY * 4; i += 4) {
            int pickedID = data[i] + data[i + 1] * 256 + data[i + 2] * 256 * 256;
            pickedIDs.push_back(pickedID);
            pickedIDs.sort();
            pickedIDs.unique();
        }

        if (pickedIDs.size() == 1 && pickedIDs.front() == 0x00ffffff) {                 //if only Background was selected
            //std::cout << "Background -- Selection Cleared" << std::endl;
            if (yRelease > screenHeight * UIpercentageBottom) {
                selManager->selection.clear();
            }
                
        }

        for (int ID : pickedIDs) {
            if (ID == 0x00ffffff) {
                continue;       //skip over background ID
            }
            else {
                if (scene_1.active) {
                    GameObject picked = scene_1.getGameObject(scene_1.SceneList, ID);

                    selManager->selection.push_back(picked);            //add picked GameObject to selection
                    selManager->selection.unique();                 //remove any GameObject twice in the list
                }
            }
        }
        
        glfwSwapBuffers(window);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xPress, &yPress);
        yPress = screenHeight - yPress;
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

#pragma region Util
glm::vec3 RGB(const float _R, const float _G, const float _B) {
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

void drawCube(const unsigned int shaderID, const GameObject toDraw) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw.position);
    model = glm::rotate(model, glm::radians(toDraw.angle), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, toDraw.scale);

    int modelloc = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

    //set color for cubes
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    if (selManager->selection.size() > 0) {             //anything is selected
        if (std::find(selManager->selection.begin(), selManager->selection.end(), toDraw) != selManager->selection.end()) {     //check if current Object to draw is in the selectedlist
            r = selManager->selectionColor.x;           //set selection color
            g = selManager->selectionColor.y;
            b = selManager->selectionColor.z;
        }
        else {
            r = toDraw.color.x;                 //set Color of the GameObject
            g = toDraw.color.y;
            b = toDraw.color.z;
        }
    }
    else {
        r = toDraw.color.x;
        g = toDraw.color.y;
        b = toDraw.color.z;
    }

    int vertexcolor = glGetUniformLocation(shaderID, "color");          //sent color values to shader
    glUniform3f(vertexcolor, r, g, b);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawCubePicking(const unsigned int shaderID, const GameObject toDraw, const int nr) {
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

void drawPlane(const unsigned int shaderID, const glm::vec2 mapSize, const glm::vec3 planeColor, const glm::vec3* planePosition) {
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
glm::vec3* initMap(const glm::vec2 mapSize)
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

glm::vec3* initMap(const int mapSizeX, const int mapSizeY)
{
    return initMap(glm::vec2(mapSizeX, mapSizeY));
}
#pragma endregion

glm::vec3 screenToWorld() {
    /* Nedded:
        - Camera Position -> fixed on (x, 10, z)
        - FOV -> 45.0f (standard)

       goal:
        - vec3 (x,0,z)
    */



    glm::vec3 lookAt = glm::vec3(0.0f);
    glm::vec3 target;
    target = cameraPos + lookAt;
    return target;
}