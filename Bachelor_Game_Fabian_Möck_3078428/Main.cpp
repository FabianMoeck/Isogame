#include "Main.h"

#define INIT_SCREENWIDTH 800                //only initial screensize
#define INIT_SCREENHEIGTH 600

#define MAPSIZE_X 50
#define MAPSIZE_Y 50
#define GRID_MULTI 1               //how much each plane part is devided in the grid (10 = 10= Nodes on 1:1 Square)

//work in Porgress

//DEBUG
bool debugDrawCubes = true;

#pragma region Variables
//Screen
int screenHeight = INIT_SCREENHEIGTH;
int screenWidth = INIT_SCREENWIDTH;

//VAO
unsigned int CUBE_VAO;
unsigned int PLANE_VAO;
unsigned int GRID_VAO;

unsigned int shaderID;

//draw Objects / Scene
Scene scene_1;
Map map;

//Building / placing new Objects
bool placeGhost = false;
GhostGO ghostBuilding;

std::list<AttackRequest*> attackList;
std::list<BuildRequest*> buildRequests;
bool barracksBuild = false;
GameObject* barrack;
bool factoryBuild = false;
GameObject* factory;

bool enemyMain = true;

//Pathfinding
std::list<PathRequest*> moveRequest;
Pathfinding pathFinding;

//Selection
SelectionManager* selManager;
double xPress = 0, yPress = 0;

//Money
int currentMoney = 0;
float timeTillNewMoney = 1.5f;
float timeSinceNewMoney = 1.5f;
int addAmount = 20;

//camera
float fov = 45.0f;
glm::mat4 view;
glm::mat4 projection;

glm::vec3 cameraPos = glm::vec3(25.0f, 20.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool moveCam = false;
double screenPercentage = 0.09;
double screenUp = 0 + (screenHeight * screenPercentage), screenDown = screenHeight - (screenHeight * screenPercentage);
double screenLeft = 0 + (screenWidth * screenPercentage), screenRight = screenWidth - (screenWidth * screenPercentage);

enum class Direction { forward, back, right, left };
void moveCamera(Direction direction);

//time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//UI
double UIpercentageRight = 0.25;
double UIright = screenWidth - (screenWidth * UIpercentageRight);
double UIpercentageBottom = 0.15;
double UIbottom = screenHeight - (screenHeight * UIpercentageBottom);
#pragma endregion

int main()
{
#pragma region Window Init / Inputcallbacks
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
#pragma endregion

    //setup shaders
    Shader shader = Shader();           //init Shader (create Shader objects and bind them)
    shaderID = shader.ID;

    glEnable(GL_DEPTH_TEST);

#pragma region Scene/GameObjects
    //init Scene and GO's
    scene_1 = Scene();
    GameObject cube1 = GameObject("TestUnit_1", glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, RGBConvert(247, 202, 22), true, GameObject::GameObjectType::FootSoldier, GameObject::Team::Player);
    GameObject cube2 = GameObject("Enemy_Barracks", glm::vec3(4.0f, 0.0f, 4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), false, GameObject::GameObjectType::Barracks, GameObject::Team::Enemy);
    GameObject cube3 = GameObject("TestUnit_2", glm::vec3(12.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 30.0f, RGBConvert(200, 78, 0), true, GameObject::GameObjectType::FootSoldier, GameObject::Team::Ally);
    GameObject cube4 = GameObject("Cube_Green", glm::vec3(5.0f, 0.0f, 9.0f), glm::vec3(1.0f, 1.0f, 1.0f), 30.0f, RGBConvert(50, 200, 10), true, GameObject::GameObjectType::Scout, GameObject::Team::Neutral);
    GameObject cube5 = GameObject("Enemy_Main", glm::vec3(15.0f, 0.0f, 19.0f), glm::vec3(2.0f, 1.0f, 2.0f), 0.0f, RGBConvert(256, 256, 256), false, GameObject::GameObjectType::MainBuilding, GameObject::Team::Enemy);

    scene_1.SceneList.push_back(&cube1);
    scene_1.SceneList.push_back(&cube2);
    scene_1.SceneList.push_back(&cube3);
    scene_1.SceneList.push_back(&cube4);
    scene_1.SceneList.push_back(&cube5);

    selManager = SelectionManager::getInstance();
    selManager->selectionColor = RGBConvert(240, 43, 69);                  //set Color that all selections are displayed in (current: Red)

    //Map
    map = Map(MAPSIZE_X,MAPSIZE_Y, RGBConvert(17.0f, 138.0f, 19.0f), GRID_MULTI);
    map.updateGrid(&scene_1.SceneList);

    //Pathfinding
    pathFinding = Pathfinding(&map);

    //vertex input into buffer
    createCubeVAO();
    createPlaneVAO();
    createGridVAO();
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
        glm::vec3 bgColor = RGBConvert(68, 135, 201);
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

        glm::vec3 direction = glm::vec3(-0.45f, -0.707106f, 0.0f);
        cameraFront = glm::normalize(direction);

        projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

        

        //render Objects
        scene_1.active = true;              //set scene as active
        if (scene_1.active && enemyMain) {
            for (GameObject* g : scene_1.SceneList)
            {
                if(debugDrawCubes)
                    drawCube(shader.ID, g);
            }
            if (placeGhost) {
                drawGhostObject(shaderID, &ghostBuilding);
            }
        }

        if(debugDrawCubes)
            drawPlane(shader.ID, &map);
        else
            drawPickingPlane(shaderID, &map);          //debug


#pragma region UI content
        //ImGui UIRight
        ImGui::Begin("Build UI");
        ImGui::SetWindowPos(ImVec2(UIright, 0.5f));
        ImGui::SetWindowSize(ImVec2((screenWidth - UIright), screenHeight));

        ImGui::Text("Money: %d", currentMoney);
        ImGui::NewLine();
        if (ImGui::BeginTabBar("tabs")) {
            if (ImGui::BeginTabItem("Buildings")) {
                if (ImGui::Button("Barracks\nCost: 300", ImVec2((screenWidth - UIright) * 0.45f, screenHeight * 0.1f))) {
                    GhostGO gGO = GhostGO("New Barracks", glm::vec3(1.0f, 1.0f, 1.0f), RGBConvert(0,0,100), false, GameObject::GameObjectType::Barracks, GameObject::Team::Player);
                    ghostBuilding = gGO;
                }
                ImGui::SameLine();
                if (ImGui::Button("Factory\nCost: 500", ImVec2((screenWidth - UIright) * 0.45, screenHeight * 0.1))) {
                    GhostGO gGO = GhostGO("New Factory", glm::vec3(2.0f, 1.0f, 2.0f), RGBConvert(113, 0, 138), false, GameObject::GameObjectType::Factory, GameObject::Team::Player);
                    ghostBuilding = gGO;
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Units")) {
                if (ImGui::Button("FootSoldier\nCost: 100", ImVec2((screenWidth - UIright) * 0.45f, screenHeight * 0.1f))) {
                    if (barracksBuild){
                        BuildRequest* b = new BuildRequest(barrack, "New FootSoldier", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, RGBConvert(247, 202, 22), true, GameObject::GameObjectType::FootSoldier, GameObject::Team::Player);
                        buildRequests.push_back(b);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Scout\nCost: 100", ImVec2((screenWidth - UIright) * 0.45f, screenHeight * 0.1f))) {
                    if (barracksBuild) {
                        BuildRequest* b = new BuildRequest(barrack, "New Scout", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, RGBConvert(200, 170, 0), true, GameObject::GameObjectType::Scout, GameObject::Team::Player);
                        buildRequests.push_back(b);
                    }
                }

                ImGui::NewLine();
                if (ImGui::Button("HeavyTank\nCost: 220", ImVec2((screenWidth - UIright) * 0.45f, screenHeight * 0.1f))) {
                    if (factoryBuild) {
                        BuildRequest* b = new BuildRequest(factory, "New HT", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, RGBConvert(196, 177, 69), true, GameObject::GameObjectType::HeavyTank, GameObject::Team::Player);
                        buildRequests.push_back(b);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("LightTank\nCost: 170", ImVec2((screenWidth - UIright) * 0.45f, screenHeight * 0.1f))) {
                    if (factoryBuild) {
                        BuildRequest* b = new BuildRequest(factory, "New LT", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, RGBConvert(166, 146, 35), true, GameObject::GameObjectType::LightTank, GameObject::Team::Player);
                        buildRequests.push_back(b);
                    }
                }

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        //BottomUI
        if (selManager->selection.size() > 0) {
            ImGui::Begin("Selection UI");
            ImGui::SetWindowPos(ImVec2(0, UIbottom));
            ImGui::SetWindowSize(ImVec2((screenWidth - (screenWidth * UIpercentageRight)), (screenHeight - UIbottom)));

            std::list<int> checkedtypes;
            std::list<GameObject*> newSel;
            for (GameObject* selected : selManager->selection) {
                ImGui::SameLine();
                if (checkedtypes.size() == 0) {
                    checkedtypes.push_back((int)selected->type);
                    if (ImGui::Button(selected->type_tostring((int)selected->type), ImVec2((screenWidth - UIright) * 0.35, (screenHeight - UIbottom) * 0.35))) {
                        for (GameObject* withType : selManager->selection) {
                            if (withType->type == selected->type) {
                                newSel.push_back(withType);
                            }
                        }
                        break;
                    }
                }
                for (int i : checkedtypes) {
                    if (i != (int)selected->type) {
                        checkedtypes.push_back((int)selected->type);
                        if (ImGui::Button(selected->type_tostring((int)selected->type), ImVec2((screenWidth - UIright) * 0.35, (screenHeight - UIbottom) * 0.35))) {
                            for (GameObject* withType : selManager->selection) {
                                if (withType->type == selected->type) {
                                    newSel.push_back(withType);
                                }
                            }
                            break;
                        }
                    }
                }
            }
            if (newSel.size() > 0) {                //if a new Selection is done by the buttons
                selManager->setSelection(newSel);
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

        if (timeSinceNewMoney >= timeTillNewMoney) {            //try to add money to account
            addMoney();
            timeSinceNewMoney = 0.0f;
        }
        else
            timeSinceNewMoney += deltaTime;

        if (moveRequest.size() > 0) {                           //move Units
            for (PathRequest* pr : moveRequest) {
                if (pr->moving) {
                    drawLine(pr);
                    pr->move(deltaTime);
                    if (!pr->moving) {
                        moveRequest.remove(pr);
                        map.updateGrid(&scene_1.SceneList);
                        break;
                    }
                }
            }
        }

        if (attackList.size() > 0) {                    //attack Units
            for (AttackRequest* atk : attackList) {
                drawLine(atk);
                int r = atk->attack(deltaTime);                 // 1 = target didnt die or not attacked (time), 0 = any unit was destroyed, 2 = Enemy main Building destroyed, 3 = Player Main destroyed
                if (r == 1)
                    continue;
                else if(r == 0) {
                    GameObject::removeObject(&scene_1.SceneList, atk->def);
                    attackList.remove(atk);
                    map.updateGrid(&scene_1.SceneList);
                    break;
                }
                else if (r == 2) {
                    std::cout << "Enemy Main Building Destroyed\nYou Win" << std::endl;
                    GameObject::removeObject(&scene_1.SceneList, atk->def);
                    attackList.remove(atk);
                    map.updateGrid(&scene_1.SceneList);
                    break;
                }
                else if (r == 3) {
                    std::cout << "Player Main Building Destroyed\nYou Loose" << std::endl;
                    GameObject::removeObject(&scene_1.SceneList, atk->def);
                    attackList.remove(atk);
                    map.updateGrid(&scene_1.SceneList);
                    break;
                }
            }
        }

        if (buildRequests.size() > 0) {                     //handle Build requests
            for (BuildRequest* br : buildRequests) {
                if (!br->handled) {
                    if(currentMoney >= br->parent->u->cost)
                        br->build(&scene_1, &deltaTime);
                    else {
                        std::cout << "Not enough money\n";
                    }
                }
                else {
                    reduceMoney(br->parent->u->cost);
                    map.updateGrid(&scene_1.SceneList);
                    buildRequests.remove(br);
                    break;
                }
            }
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
    
    if (key == GLFW_KEY_LEFT_CONTROL) ctrl_Pressed = action == GLFW_PRESS;
    //save Selection
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

    if (key == GLFW_KEY_Q && action != GLFW_RELEASE) {
        if (ghostBuilding.name != "") {
            ghostBuilding.rotate(-30 * (1 - deltaTime));
        }
    }

    if (key == GLFW_KEY_E && action != GLFW_PRESS) {
        if (ghostBuilding.name != "") {
            ghostBuilding.rotate(30 * (1 - deltaTime));
        }
    }

    //DEBUG
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        debugDrawCubes = !debugDrawCubes;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {  
    if (ypos < screenUp || ypos > screenDown || xpos > screenRight || xpos < screenLeft)
        moveCam = true;
    else
        moveCam = false;

    if (ghostBuilding.name != "") {
        ghostPosition(window, &ghostBuilding, &map);
        placeGhost = true;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double pixelX = 1, pixelY = 1;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        //Selection
        if (ghostBuilding.name == "") {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderID);

            int projectloc = glGetUniformLocation(shaderID, "projection");
            glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
            int viewloc = glGetUniformLocation(shaderID, "view");
            glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

            int objects = 0;
            for (GameObject* Go : scene_1.SceneList)
            {
                if (Go->selectable && Go->team == GameObject::Team::Player)
                    drawCubePicking(shaderID, Go, objects);
                objects++;
            }

            glFlush();
            glFinish();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            double xRelease, yRelease;
            glfwGetCursorPos(window, &xRelease, &yRelease);
            yRelease = screenHeight - yRelease;

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
                        GameObject* picked = scene_1.getGameObject(&scene_1.SceneList, ID);

                        selManager->selection.push_back(picked);            //add picked GameObject to selection
                        selManager->selection.unique();                 //remove any GameObject twice in the list
                    }
                }
            }

            glfwSwapBuffers(window);
        }

        //Building
        if (ghostBuilding.name != "") {
            if (ghostBuilding.tmp_position.y != -10) {
                GameObject* ghostToGo = ghostBuilding.placeGhost();
                if (reduceMoney(ghostToGo->u->cost)) {
                    ghostBuilding = GhostGO();
                    placeGhost = false;
                    scene_1.SceneList.push_back(ghostToGo);
                    map.updateGrid(&scene_1.SceneList);

                    if (ghostToGo->type == GameObject::GameObjectType::Barracks) {
                        barrack = ghostToGo;
                        barracksBuild = true;
                    }
                    else if (ghostToGo->type == GameObject::GameObjectType::Factory) {
                        factory = ghostToGo;
                        factoryBuild = true;
                    }
                }
                else
                    delete(ghostToGo);
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xPress, &yPress);
        yPress = screenHeight - yPress;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        if (ghostBuilding.name != "") {
            placeGhost = false;
            ghostBuilding = GhostGO();
        }

        if (selManager->selection.size() > 0) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shaderID);

            int projectloc = glGetUniformLocation(shaderID, "projection");
            glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
            int viewloc = glGetUniformLocation(shaderID, "view");
            glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

            int cubeNR = 0;
            for (GameObject* g : scene_1.SceneList) {
                if (!(g->team == GameObject::Team::Player) || !(g->team == GameObject::Team::Ally)) {
                    drawCubePicking(shaderID, g, cubeNR);
                    cubeNR++;
                }
            }

            drawPickingPlane(shaderID, &map);

            glFlush();
            glFinish();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            double xRelease, yRelease;
            glfwGetCursorPos(window, &xRelease, &yRelease);
            yRelease = screenHeight - yRelease;

            unsigned char* data = new unsigned char[4];
            glReadPixels(xRelease, yRelease, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            int picked_r = data[0];
            int picked_X = data[1];
            int picked_Y = data[2];

            if (picked_X + picked_Y == 510)
                std::cout << "not clicked on Map" << std::endl;
            
            else if (picked_r != 0 || picked_r == 0 && picked_X == 0 && picked_Y == 0) {        //any GO was selected as move target
                GameObject* attacking = scene_1.getGameObject(&scene_1.SceneList, picked_r);
                if (attacking->team == GameObject::Team::Enemy) {
                    for (GameObject* selected : selManager->selection) {
                        ITroop* t = (ITroop*)selected->u;
                        float r = t->range;

                        if (glm::distance(selected->position, attacking->position) > r) {
                            if (moveRequest.size() > 0) {
                                for (PathRequest* const pr : moveRequest) {
                                    if (pr->moveGO == selected) {
                                        moveRequest.remove(pr);
                                        break;
                                    }
                                }
                            }
                            PathRequest* pr = new PathRequest(selected, attacking, pathFinding, &attackList);
                            moveRequest.push_back(pr);
                        }
                    }
                }
            }
            else {
                for (GameObject* selected : selManager->selection) {
                    if (moveRequest.size() > 0) {
                        for (PathRequest* const pr : moveRequest) {
                            if (pr->moveGO == selected) {
                                moveRequest.remove(pr);
                                break;
                            }
                        }
                    }
                    PathRequest* pr = new PathRequest(selected, map.grid[picked_X][picked_Y]->position, pathFinding, &attackList);
                    moveRequest.push_back(pr);
                }
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

#pragma region Util
glm::vec3 RGBConvert(const float _R, const float _G, const float _B) {
    float r = _R / 255;
    float g = _G / 255;
    float b = _B / 255;

    return glm::vec3(r, g, b);
}

//overloading << for vec3
std::ostream& operator<<(std::ostream& os, const glm::vec3 vec)
{
    os << vec.x << "/" << vec.y << "/" << vec.z;
    return os;
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

void createGridVAO() {
    float planeVertices[] = {
    -0.5f / GRID_MULTI, -0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
     0.5f / GRID_MULTI, -0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
     0.5f / GRID_MULTI,  0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
     0.5f / GRID_MULTI,  0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
    -0.5f / GRID_MULTI,  0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
    -0.5f / GRID_MULTI, -0.5f / GRID_MULTI, -0.5f / GRID_MULTI,
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &GRID_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(GRID_VAO);

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

void drawCube(const unsigned int shaderID, const GameObject* toDraw) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw->position);
    model = glm::rotate(model, glm::radians(toDraw->angle), glm::vec3(0, 1, 0));
    model = glm::scale(model, toDraw->scale);

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
            r = toDraw->color.x;                 //set Color of the GameObject
            g = toDraw->color.y;
            b = toDraw->color.z;
        }
    }
    else {
        r = toDraw->color.x;
        g = toDraw->color.y;
        b = toDraw->color.z;
    }

    int vertexcolor = glGetUniformLocation(shaderID, "color");          //sent color values to shader
    glUniform3f(vertexcolor, r, g, b);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawCubePicking(const unsigned int shaderID, const GameObject* toDraw, const int nr) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw->position);
    model = glm::rotate(model, glm::radians(toDraw->angle), glm::vec3(0, 1, 0));
    model = glm::scale(model, toDraw->scale);

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

void drawPlane(const unsigned int shaderID, const Map *map) {
    glBindVertexArray(PLANE_VAO);
    for (unsigned int i = 0; i < map->mapSize.x * map->mapSize.y; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, map->mapPosition[i]);
        float angle = 90;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

        int modelloc = glGetUniformLocation(shaderID, "model");
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

        float r = map->mapColor.x;
        float g = map->mapColor.y;
        float b = map->mapColor.z;
        int vertexcolor = glGetUniformLocation(shaderID, "color");
        glUniform3f(vertexcolor, r, g, b);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void drawPickingPlane(const unsigned int shaderID, const Map *map) {
    glBindVertexArray(GRID_VAO);
    for (unsigned int i = 0; i < map->mapSize.x * GRID_MULTI; i++) {
        for (unsigned int j = 0; j < map->mapSize.y * GRID_MULTI; j++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, map->grid[i][j]->position);
            float angle = 90;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

            int modelloc = glGetUniformLocation(shaderID, "model");
            glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

            //set color for cubes
            float r = 0.0f;               //ignore red
            int g = (i * 256 & 0x0000FF00) >> 8;
            int b = (j * (256*256) & 0x00FF0000) >> 16;

            if (!map->grid[i][j]->walkable) {
                r = 255.0f;
            }

            int vertexcolor = glGetUniformLocation(shaderID, "color");
            glUniform3f(vertexcolor, r / 255.0f, g / 255.0f, b / 255.0f);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

void drawGhostObject(unsigned int shaderID, GhostGO* toDraw) {
    glBindVertexArray(CUBE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, toDraw->tmp_position);
    model = glm::rotate(model, glm::radians(toDraw->angle), glm::vec3(0, 1, 0));
    model = glm::scale(model, toDraw->scale);

    int modelloc = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

    //set color for cubes
    float r = toDraw->color.x;
    float g = toDraw->color.y;
    float b = toDraw->color.z;

    int vertexcolor = glGetUniformLocation(shaderID, "color");          //sent color values to shader
    glUniform3f(vertexcolor, r, g, b);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ghostPosition(GLFWwindow* window, GhostGO* ghost, Map* map) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderID);

    int projectloc = glGetUniformLocation(shaderID, "projection");
    glUniformMatrix4fv(projectloc, 1, GL_FALSE, glm::value_ptr(projection));
    int viewloc = glGetUniformLocation(shaderID, "view");
    glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));

    drawPickingPlane(shaderID, map);

    glFlush();
    glFinish();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    double xRelease, yRelease;
    glfwGetCursorPos(window, &xRelease, &yRelease);
    yRelease = screenHeight - yRelease;

    unsigned char* data = new unsigned char[4];
    glReadPixels(xRelease, yRelease, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    int picked_X = data[1];
    int picked_Y = data[2];
    if (picked_X + picked_Y != 510) {
        glm::vec3 pos = glm::vec3(map->grid[picked_X][picked_Y]->position.x, map->grid[picked_X][picked_Y]->position.y + (ghost->scale.y), map->grid[picked_X][picked_Y]->position.z);
        ghost->tmp_position = pos;
    }
}

void drawLine(PathRequest* pr) {
    unsigned int LINE_VAO;
    for (unsigned int i = 0; i < pr->path.size(); i++)
    {
        unsigned int VBO;
        glGenVertexArrays(1, &LINE_VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(LINE_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glLineWidth(3);
        glm::vec3* c = get(&pr->path, i);
        if (pr->current == 0 && i == 0) {                                //problem second waypoint doesnt get connected immidiatly
            float lineVertices[] = {
            pr->moveGO->position.x, pr->moveGO->position.y, pr->moveGO->position.z,
            c->x, pr->moveGO->position.y, c->z,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        }
        else {
            if (i < pr->path.size() - 1 && pr->current > 0) {
                glm::vec3* n = get(&pr->path, i+1);
                float lineVertices[] = {
                n->x, pr->moveGO->position.y, n->z,
                c->x, pr->moveGO->position.y, c->z,
                };
                glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
            }
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &VBO);

        glBindVertexArray(LINE_VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3());
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(1.0f));

        int modelloc = glGetUniformLocation(shaderID, "model");
        glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

        int vertexcolor = glGetUniformLocation(shaderID, "color");
        glUniform3f(vertexcolor, 1, 1, 1);

        glDrawArrays(GL_LINES, 0, 2);
    }
}

void drawLine(AttackRequest* ar) {
    unsigned int LINE_VAO;
    unsigned int VBO;
    glGenVertexArrays(1, &LINE_VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(LINE_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glLineWidth(3);

    GameObject* atk = GameObject::getGameObjectFromUnit(&scene_1.SceneList, ar->atk);
    GameObject* def = GameObject::getGameObjectFromUnit(&scene_1.SceneList, ar->def);

    float lineVertices[] = {
    atk->position.x, atk->position.y, atk->position.z,
    def->position.x, def->position.y, def->position.z
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    glBindVertexArray(LINE_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3());
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1.0f));

    int modelloc = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));

    int vertexcolor = glGetUniformLocation(shaderID, "color");
    glUniform3f(vertexcolor, 0.96f, 0.74f, 0.21f);

    glDrawArrays(GL_LINES, 0, 2);
    
}
#pragma endregion

#pragma region Gameplay
void moveCamera(Direction direction) {
    float cameraSpeed = 10.0f * deltaTime;                   //adjust accordingly
    glm::vec3 xzPlane = glm::vec3(1.0f, 0.0f, 1.0f);              //vector on xz plane
    if (direction == Direction::forward && cameraPos.x > 28.0f)
        cameraPos += cameraSpeed * xzPlane * cameraFront;
    if (direction == Direction::back && cameraPos.x < MAPSIZE_Y + 3.0f)
        cameraPos -= cameraSpeed * xzPlane * cameraFront;
    if (direction == Direction::left && cameraPos.z < MAPSIZE_X - 10.0f)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (direction == Direction::right && cameraPos.z > 7.5f)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void addMoney() {
    currentMoney += addAmount;
}

bool reduceMoney(int _amount) {
    if (currentMoney - _amount >= 0) {
        currentMoney -= _amount;
        return true;
    }
    else {
        std::cout << "nout enough money\n";
        return false;
    }
}

#pragma endregion
