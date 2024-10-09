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
#include "glframework/materials/include/depthMaterial.h"
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
                                                     0.1f, 1000.0f);

        cameraControl = std::make_shared<GameCameraController>();
        cameraControl->setCamera(camera.get());
        // 智能指针安全向下转型
        std::dynamic_pointer_cast<GameCameraController>(cameraControl)->setSpeed(0.4);
}


void prepareAll()
{
        renderer = std::make_unique<Renderer>();
        scene = std::make_shared<Scene>();

        /* 方块A */
        /* 创建一个几何方块 */
        auto geometry = Geometry::createBox(4.0f);
        auto materialA = std::make_shared<PhongMaterial>();
        materialA->setDiffuse(std::make_shared<Texture>("assets/textures/dog_1.jpg", 0));
        materialA->mStencilTest = true;
        materialA->mSFail = GL_KEEP;
        materialA->mZFail = GL_KEEP;
        materialA->mZPass = GL_REPLACE; // 都通过后替换为0x1 (materialA->mStencilRef)
        materialA->mStencilFunc = GL_ALWAYS;
        materialA->mStencilRef = 0x1;
        materialA->mStencilFuncMask = 0xFF;
        materialA->mStencilMask = 0xFF; // 这里开启防止前一个关闭了写入
        auto meshA = std::make_shared<Mesh>(geometry, materialA);
        scene->addChild(meshA);

        /* 创建一个勾边方块 */
        auto materialBound = std::make_shared<WhiteMaterial>();
        materialBound->mDepthTest = false; //如果共享边界，需要关闭深度测试,这样物体能覆盖到边界
        materialBound->mStencilTest = true;
        materialBound->mSFail = GL_KEEP;
        materialBound->mZFail = GL_KEEP;
        materialBound->mZPass = GL_KEEP;
        // 物体都是等于0x1的模板值，这里不等于1就说明是边界
        materialBound->mStencilFunc = GL_NOTEQUAL;
        materialBound->mStencilRef = 0x1;
        materialBound->mStencilFuncMask = 0xFF;
        materialBound->mStencilMask = 0x0; // 边界覆盖的区域不应该写入模板
        auto meshABound = std::make_shared<Mesh>(geometry, materialBound);
        meshABound->setPosition(meshA->getPosition());
        meshABound->setScale(glm::vec3(1.2f, 1.2f, 1.2f));
        scene->addChild(meshABound);

        /* 方块B */
        /* 创建一个几何方块 */
        auto geometryB = Geometry::createBox(4.0f);
        auto materialB = std::make_shared<PhongMaterial>();
        materialB->setDiffuse(std::make_shared<Texture>("assets/textures/wall.jpg", 0));
        materialB->mStencilTest = true;
        materialB->mSFail = GL_KEEP;
        materialB->mZFail = GL_KEEP;
        materialB->mZPass = GL_REPLACE; // 都通过后替换为0x1 (materialA->mStencilRef)
        materialB->mStencilFunc = GL_ALWAYS;
        materialB->mStencilRef = 0x1;
        materialB->mStencilFuncMask = 0xFF;
        materialB->mStencilMask = 0xFF; // 这里开启防止前一个关闭了写入
        auto meshB = std::make_shared<Mesh>(geometryB, materialB);
        meshB->setPosition(glm::vec3(3.0f, 1.0f, 1.0f));
        scene->addChild(meshB);

        /* 创建一个勾边方块 */
        auto materialBBound = std::make_shared<WhiteMaterial>();
        materialBBound->mDepthTest = false;
        materialBBound->mStencilTest = true;
        materialBBound->mSFail = GL_KEEP;
        materialBBound->mZFail = GL_KEEP;
        materialBBound->mZPass = GL_KEEP;
        // 物体都是等于0x1的模板值，这里不等于1就说明是边界
        materialBBound->mStencilFunc = GL_NOTEQUAL;
        materialBBound->mStencilRef = 0x1;
        materialBBound->mStencilFuncMask = 0xFF;
        materialBBound->mStencilMask = 0x0; // 边界覆盖的区域不应该写入模板
        auto meshBBound = std::make_shared<Mesh>(geometryB, materialBBound);
        meshBBound->setPosition(meshB->getPosition());
        meshBBound->setScale(glm::vec3(1.2f, 1.2f, 1.2f));
        scene->addChild(meshBBound);

        /* 创建平行光 */
        directionalLight = std::make_shared<DirectionalLight>();
        directionalLight->setDirection(glm::vec3(-1.0f));
        directionalLight->setSpecularIntensity(0.1f);

        /* 创建环境光 */
        ambLight = std::make_shared<AmbientLight>();
        ambLight->setColor(glm::vec3(0.2f));

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
        // auto xPos = static_cast<float>(glm::sin(glfwGetTime()) + 3.0f);
        // whiteSpotMesh->setPosition(glm::vec3(xPos,
        //                                      whiteSpotMesh->getPosition().y,
        //                                      whiteSpotMesh->getPosition().z));
        // spotLight->setPosition(whiteSpotMesh->getPosition());
        //
        // xPos = static_cast<float>(glm::sin(glfwGetTime()) - 3.0f);
        // whitePointMesh->setPosition(glm::vec3(xPos,
        //                                       whitePointMesh->getPosition().y,
        //                                       whitePointMesh->getPosition().z));
        // pointLights[0]->setPosition(whitePointMesh->getPosition());

        // meshes[0]->rotateY(1.0f);
}

void render()
{
        renderer->setClearColor(clearColor);
        renderer->render(scene, camera, directionalLight, pointLights, spotLight, ambLight);
}

