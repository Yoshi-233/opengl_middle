//
// Created by nio on 24-9-18.
#include "glframework/include/core.h"
#include "glframework/include/shader.hpp"
#include "glframework/include/texture.h"
#include "glframework/include/geometry.h"
#include "glframework/renderer/include/renderer.h"
#include "application/app/Application.h"
#include "common/include/check_err.h"
#include "common/include/common.h"

#include "application/camera/include/camera.h"
#include "application/camera/include/orthographicCamera.h"
#include "application/camera/include/perspectiveCamera.h"
#include "application/camera/include/trackballCameraController.h"
#include "application/camera/include/gameCameraController.h"
#include "application/assimpLoader/include/assimpLoader.h"

#include "glframework/materials/include/phongMaterial.h"
#include "glframework/materials/include/whiteMaterial.h"
#include "glframework/include/mesh.h"
#include "glframework/include/scene.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

glm::mat4 transform{1.0f};

std::unique_ptr<Renderer> renderer;
std::shared_ptr<Mesh> whiteSpotMesh;
std::shared_ptr<Mesh> whitePointMesh;
std::vector<std::shared_ptr<Mesh>> meshes{};
std::shared_ptr<Scene> scene;

#define POINT_LIGHT_NUM 4
std::vector<std::shared_ptr<PointLight>> pointLights;
std::shared_ptr<SpotLight> spotLight;
std::shared_ptr<DirectionalLight> directionalLight;
std::shared_ptr<AmbientLight> ambLight;

std::shared_ptr<Camera> camera;
std::shared_ptr<CameraControl> cameraControl;

glm::vec3 clearColor{0.0f, 0.0f, 0.0f};


static void prepareCamera()
{
        camera = std::make_shared<PerspectiveCamera>(60.0f,
                                                     (float) APP.getWidth() / (float) APP.getHeight(),
                                                     0.1f, 1000.f);

        cameraControl = std::make_shared<GameCameraController>();
        cameraControl->setCamera(camera.get());
        // 智能指针安全向下转型
        std::dynamic_pointer_cast<GameCameraController>(cameraControl)->setSpeed(0.4);
}


void prepareAll()
{
        auto model = AssimpLoader::loadModel("assets/fbx/bag/backpack.obj");
        renderer = std::make_unique<Renderer>();
        scene = std::make_shared<Scene>();
        model->setScale(glm::vec3(0.5f));
        scene->addChild(model);

        /* 1. 创建环境光light */
        ambLight = std::make_shared<AmbientLight>();
        ambLight->setColor(glm::vec3(0.2f));

        /* 2. 创建探照灯 */
        auto whiteGeometry = Geometry::createSphere(0.1f);
        auto whiteMaterial = std::make_shared<WhiteMaterial>();
        whiteSpotMesh = std::make_shared<Mesh>(whiteGeometry, whiteMaterial);
        whiteSpotMesh->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
        // meshes.push_back(whiteSpotMesh);
        scene->addChild(whiteSpotMesh);

        spotLight = std::make_shared<SpotLight>();
        spotLight->setPosition(whiteSpotMesh->getPosition());
        spotLight->setTargetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
        spotLight->setInnerAngle(30.0f);
        spotLight->setOuterAngle(45.0f);

        /* 3. 创建平行光 */
        directionalLight = std::make_shared<DirectionalLight>();
        directionalLight->setDirection(glm::vec3(-1.0f, -1.0f, 0.0f));
        directionalLight->setSpecularIntensity(0.1f);

        /* 4. 创建点光源 */
        auto whitePointGeometry = Geometry::createSphere(0.1f);
        auto whitePointMaterial = std::make_shared<WhiteMaterial>();
        whitePointMesh = std::make_shared<Mesh>(whitePointGeometry, whitePointMaterial);
        whitePointMesh->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
        // meshes.push_back(whitePointMesh);
        scene->addChild(whitePointMesh);

        auto pointLight = std::make_shared<PointLight>();
        pointLight->setPosition(whitePointMesh->getPosition());
        pointLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f)); // 红色点光源
        pointLight->setParameters(65.0f);
        pointLights.push_back(std::move(pointLight));

        pointLight = std::make_shared<PointLight>();
        pointLight->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
        pointLight->setColor(glm::vec3(0.0f, 1.0f, 0.0f)); // 绿色点光源
        pointLight->setParameters(65.0f);
        pointLights.push_back(std::move(pointLight));

        pointLight = std::make_shared<PointLight>();
        pointLight->setPosition(glm::vec3(0.0f, -3.0f, 0.0f));
        pointLight->setColor(glm::vec3(0.0f, 0.0f, 1.0f)); // 蓝色点光源
        pointLight->setParameters(65.0f);
        pointLights.push_back(std::move(pointLight));

        pointLight = std::make_shared<PointLight>();
        pointLight->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
        pointLight->setColor(glm::vec3(1.0f, 1.0f, 0.0f)); // 黄色点光源
        pointLight->setParameters(65.0f);
        pointLights.push_back(std::move(pointLight));

        prepareCamera();
}

// 窗口大小改变回调函数
static void OnResize(int width, int height)
{
        GL_CHECK_ERR(glViewport(0, 0, width, height));
}

static void OnKeyBoard(int key, int scancode, int action, int mode)
{
        cameraControl->onKey(key, action, mode);
}

static void OnMouse(int button, int action, int mods)
{
        auto xPos = std::make_shared<double>();
        auto yPos = std::make_shared<double>();
        APP.getCursorPos(xPos.get(), yPos.get());

        cameraControl->onMouse(button, action, *xPos, *yPos);
}

static void OnCursorPos(double xpos, double ypos)
{
        cameraControl->onCursor(xpos, ypos);
}

static void OnScroll(double yoffset)
{
        cameraControl->onScroll(static_cast<float>(yoffset));
}

void setAPPFunctions()
{
        APP.setResizeCallback(OnResize);
        APP.setKeyCallback(OnKeyBoard);
        APP.setMouseButtonCallback(OnMouse);
        APP.setCursorPosCallback(OnCursorPos);
        APP.setScrollCallback(OnScroll);
}

void initImgui()
{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext(); // 创建imgui上下文
        ImGui::StyleColorsDark(); // 主题风格

        // 设置imgui与glfw, opengl的链接
        ImGui_ImplGlfw_InitForOpenGL(APP.getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410 core");
}

// render当前的Imgui
void renderImgui()
{
        /* 1. 开启渲染Imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* 2. 决定当前有哪些控件，从上到下 */
        ImGui::Begin("hello");
        ImGui::Text("Hello, world!");
        ImGui::Button("test button", ImVec2(40, 20));
        ImGui::ColorEdit3("clear color", glm::value_ptr(clearColor)); // 初始值为黑色
        ImGui::End();

        /* 3. 执行渲染 */
        ImGui::Render();
        int display_w, display_h;
        // 获取窗口大小
        glfwGetFramebufferSize(APP.getWindow(), &display_w, &display_h);
        // 重置窗口
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void doTransform()
{
        auto xPos = static_cast<float>(glm::sin(glfwGetTime()) + 3.0f);
        whiteSpotMesh->setPosition(glm::vec3(xPos,
                                             whiteSpotMesh->getPosition().y,
                                             whiteSpotMesh->getPosition().z));
        spotLight->setPosition(whiteSpotMesh->getPosition());

        xPos = static_cast<float>(glm::sin(glfwGetTime()) - 3.0f);
        whitePointMesh->setPosition(glm::vec3(xPos,
                                              whitePointMesh->getPosition().y,
                                              whitePointMesh->getPosition().z));
        pointLights[0]->setPosition(whitePointMesh->getPosition());

        // meshes[0]->rotateY(1.0f);
}

void render()
{
        renderer->setClearColor(clearColor);
        renderer->render(scene, camera, directionalLight, pointLights, spotLight, ambLight);
}

