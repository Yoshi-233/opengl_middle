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
#include "application/assimpLoader/include/assimpInstanceLoader.h"

#include "glframework/materials/include/phongMaterial.h"
#include "glframework/materials/include/phongEnvMaterial.h"
#include "glframework/materials/include/phongInstancedMaterial.h"
#include "glframework/materials/include/grassInstancedMaterial.h"
#include "glframework/materials/include/opacityMaskMaterial.h"
#include "glframework/materials/include/whiteMaterial.h"
#include "glframework/materials/include/depthMaterial.h"
#include "glframework/materials/include/screenMaterial.h"
#include "glframework/materials/include/cubeMaterial.h"
#include "glframework/mesh/include/mesh.h"
#include "glframework/mesh/include/instanceMesh.h"
#include "glframework/include/scene.h"
#include "glframework/framebuffer/include/framebuffer.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

int SCREEN_WIDTH = 1600;
int SCREEN_HEIGHT = 1200;

glm::mat4 transform{1.0f};

std::unique_ptr<Renderer> renderer;
std::shared_ptr<Mesh> whiteSpotMesh;
std::shared_ptr<Mesh> whitePointMesh;
std::vector<std::shared_ptr<Mesh>> meshes{};
std::shared_ptr<Scene> sceneOffScreen;
std::shared_ptr<Scene> sceneInScreen;

#define POINT_LIGHT_NUM 4
std::vector<std::shared_ptr<PointLight>> pointLights;
std::shared_ptr<SpotLight> spotLight;
std::shared_ptr<DirectionalLight> directionalLight;
std::shared_ptr<AmbientLight> ambLight;

std::shared_ptr<Camera> camera;
std::shared_ptr<CameraControl> cameraControl;

glm::vec3 clearColor{0.0f, 0.0f, 0.0f};

std::shared_ptr<Framebuffer> framebuffer;

std::shared_ptr<GrassInstancedMaterial> grassMaterial;

static void prepareCamera()
{
        camera = std::make_shared<PerspectiveCamera>(60.0f,
                                                     (float) APP.getWidth() / (float) APP.getHeight(),
                                                     0.1f, 1000.0f);

        cameraControl = std::make_shared<GameCameraController>();
        cameraControl->setCamera(camera.get());
        // 智能指针安全向下转型
        std::dynamic_pointer_cast<GameCameraController>(cameraControl)->setSpeed(0.2);
}

void setModelBlend(std::shared_ptr<Object> &obj, bool blend, float opacity)
{
        if (obj->getType() == ObjectType::MESH) {
                auto mesh = std::dynamic_pointer_cast<Mesh>(obj);
                auto material = mesh->getMaterial();
                material->mBlend = blend;
                material->mOpacity = opacity;
                material->mDepthWrite = !blend;
        }

        for (auto child: obj->getChildren()) {
                setModelBlend(child, blend, opacity);
        }
}

static void setInstancedMatrix(std::shared_ptr<Object> &obj, int index, glm::mat4 matrix)
{
        if (obj->getType() == ObjectType::INSTANCED_MESH) {
                auto instancedMesh = std::dynamic_pointer_cast<InstanceMesh>(obj);
                instancedMesh->instanceData[index] = matrix;
        }

        auto children = obj->getChildren();
        for (auto child: children) {
                setInstancedMatrix(child, index, matrix);
        }
}

static void updateInstancedMatrix(std::shared_ptr<Object> &obj)
{
        if (obj->getType() == ObjectType::INSTANCED_MESH) {
                auto instancedMesh = std::dynamic_pointer_cast<InstanceMesh>(obj);
                instancedMesh->updataMatrices();
        }

        auto children = obj->getChildren();
        for (auto child: children) {
                updateInstancedMatrix(child);
        }
}

static void setInstancedMaterial(std::shared_ptr<Object> &obj, std::shared_ptr<Material> material)
{
        if (obj->getType() == ObjectType::INSTANCED_MESH) {
                auto instancedMesh = std::dynamic_pointer_cast<InstanceMesh>(obj);
                instancedMesh->mMaterial = material;
        }

        auto children = obj->getChildren();
        for (auto child: children) {
                setInstancedMaterial(child, material);
        }
}

void prepareAll()
{
        // 必须先准备fbo再准备其他资源
        framebuffer = std::make_shared<Framebuffer>(SCREEN_WIDTH, SCREEN_HEIGHT);

        renderer = std::make_unique<Renderer>();
        // sceneOffScreen = std::make_shared<Scene>();
        sceneInScreen = std::make_shared<Scene>();

        auto boxGeo = Geometry::createBox(1.0f);
        auto boxMat = std::make_shared<CubeMaterial>();
        boxMat->mDiffuse = std::make_shared<Texture>("assets/textures/bk.jpg", 0);
        auto boxMesh = std::make_shared<Mesh>(boxGeo, boxMat);
        sceneInScreen->addChild(boxMesh);

        auto sphereGeo = Geometry::createSphere(4.0f);
        auto sphereMat = std::make_shared<PhongInstancedMaterial>();
        sphereMat->setDiffuse(std::make_shared<Texture>("assets/textures/earth.png", 0));

        int rNum = 10;
        int cNum = 10;
        auto grassModel = AssimpInstanceLoader::loadModel(
                "assets/fbx/grassNew.obj", rNum * cNum);
        glm::mat4 translate;
        glm::mat4 rotate;
        glm::mat4 transformMat;
        srand(glfwGetTime());

        for (int i = 0; i < rNum; i++) {
                for (int j = 0; j < cNum; j++) {
                        translate = glm::translate(glm::mat4(1.0f),
                                                   glm::vec3((float) i * 0.2f, 0.0f, (float) j * 0.2f));
                        rotate = glm::rotate(glm::radians((float) (rand() % 90)), glm::vec3(0.0f, 1.0f, 0.0f));
                        setInstancedMatrix(grassModel, i * cNum + j, translate * rotate);
                }
        }
        updateInstancedMatrix(grassModel);

        grassMaterial = std::make_shared<GrassInstancedMaterial>();
        grassMaterial->setDiffuse(std::make_shared<Texture>("assets/textures/GRASS.PNG", 0));
        grassMaterial->setOpacityMask(std::make_shared<Texture>("assets/textures/grassMask.png", 2));
        grassMaterial->mCloudMask = std::make_shared<Texture>("assets/textures/CLOUD.PNG", 3);
        grassMaterial->mBlend = true;
        grassMaterial->mDepthWrite = false;
        setInstancedMaterial(grassModel,
                             std::dynamic_pointer_cast<Material>(grassMaterial));
        sceneInScreen->addChild(grassModel);

        auto house = AssimpLoader::loadModel("assets/fbx/house.fbx");
        house->setScale(glm::vec3(0.5f));
        house->setPosition(glm::vec3((float)rNum * 0.2f / 2.0f, 0.4f,
                                     (float)cNum * 0.2f / 2.0f));
        sceneInScreen->addChild(house);


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
        ImGui::Begin("GrassMaterialEditor");

        ImGui::Text("Grass Material Editor");
        ImGui::SliderFloat("UVscale", &grassMaterial->mUVScale, 0.0f, 100.0f);
        ImGui::InputFloat("brightness", &grassMaterial->mBrightness);

        ImGui::Text("Wind");
        ImGui::InputFloat("windStrength", &grassMaterial->mWindStrength);
        ImGui::InputFloat("phaseScale", &grassMaterial->mPhaseScale);
        ImGui::ColorEdit3("windDirection", (float *) &grassMaterial->mWindDirection);

        ImGui::Text("Cloud");
        ImGui::ColorEdit3("cloudWhiteColor", (float *) &grassMaterial->mCloudWhiteColor);
        ImGui::ColorEdit3("cloudBlackColor", (float *) &grassMaterial->mCloudBlackColor);
        ImGui::SliderFloat("cloudUVScale", &grassMaterial->mCloudUVScale, 0.0f, 100.0f);
        ImGui::InputFloat("cloudSpeed", &grassMaterial->mCloudSpeed);
        ImGui::SliderFloat("cloudLerp", &grassMaterial->mCloudLerp, 0.0f, 1.0f);

        ImGui::Text("Light");
        ImGui::InputFloat("Intensity", &directionalLight->mIntensity);

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

        /* 每一帧中都需要调用两次渲染
         * pass01: 离屏渲染，将物体渲染到ColorAttachement的FBO中
         * pass02: 贴屏渲染，将FBO中的ColorAttachement纹理贴到屏幕上
         * */
        renderer->render(sceneOffScreen,
                         camera, directionalLight, pointLights, spotLight, ambLight,
                         framebuffer->mFBO);
        // 使用默认的fbo = 0
        renderer->render(sceneInScreen,
                         camera, directionalLight, pointLights, spotLight, ambLight);
}

