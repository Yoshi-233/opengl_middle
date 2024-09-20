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

#include "glframework/materials/include/phongMaterial.h"
#include "glframework/materials/include/whiteMaterial.h"
#include "glframework/include/mesh.h"

glm::mat4 transform{1.0f};

std::unique_ptr<Renderer> renderer;
std::shared_ptr<Mesh> whiteMesh;
std::vector<std::shared_ptr<Mesh>> meshes{};
std::shared_ptr<PointLight> pointLight;
std::shared_ptr<AmbientLight> ambLight;

std::shared_ptr<Camera> camera;
std::shared_ptr<CameraControl> cameraControl;


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
        renderer = std::make_unique<Renderer>();

        /* 1.创建geometry */
        auto geometry = Geometry::createSphere(2.0f);

        /* 2. 创建material配置参数 */
        auto material = std::make_shared<PhongMaterial>();
        material->setShiness(32.0);
        material->setDiffuse(std::make_shared<Texture>("assets/textures/dog_1.jpg", 0));

        /* 3. 生成mesh*/
        auto mesh = std::make_shared<Mesh>(geometry, material);
        mesh->setPosition(glm::vec3(4.0f, 0.0f, 0.0f));
        meshes.push_back(mesh);

        /* 4. 创建light */
        pointLight = std::make_shared<PointLight>();
        ambLight = std::make_shared<AmbientLight>();
        ambLight->setColor(glm::vec3(0.2f));

        /* 5. 创建点光源 */
        auto whiteGeometry = Geometry::createSphere(1.5f);
        auto whiteMaterial = std::make_shared<WhiteMaterial>();
        whiteMesh = std::make_shared<Mesh>(whiteGeometry, whiteMaterial);
        whiteMesh->setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setPosition(whiteMesh->getPosition());
        pointLight->setParameters(65.0);

        meshes.push_back(whiteMesh);

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

void doTransform()
{
        auto xPos = static_cast<float>(glm::sin(glfwGetTime()) + 3.0f);
        whiteMesh->setPosition(glm::vec3(xPos,
                                         whiteMesh->getPosition().y,
                                         whiteMesh->getPosition().z));
        pointLight->setPosition(whiteMesh->getPosition());
        // transform = glm::rotate(transform, 0.03f, glm::vec3(0.0f, 1.0f, 1.0f));
}

void render()
{
        renderer->render(meshes, camera, pointLight, ambLight);
}

