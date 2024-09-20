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
#include "glframework/include/mesh.h"

glm::mat4 transform{1.0f};

std::unique_ptr<Renderer> renderer;
std::vector<std::shared_ptr<Mesh>> meshes{};
std::shared_ptr<DirectionalLight> dirLight;
std::shared_ptr<AmbientLight> ambLight;

std::shared_ptr<Camera> camera;
std::shared_ptr<CameraControl> cameraControl;


static void prepareCamera()
{
        camera = std::make_shared<PerspectiveCamera>(60.0f,
                                                     (float) APP.getWidth() / (float) APP.getHeight(),
                                                     0.1f, 1000.f);

        // // 注意必须包含相机和图像
        // int size = 6.0f;
        // camera = std::make_unique<OrthographicCamera>(-size, size,
        //                                               -size, size,
        //                                               size, -size);

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
        auto material_01 = std::make_shared<PhongMaterial>();
        material_01->setShiness(32.0);
        material_01->setDiffuse(std::make_shared<Texture>("assets/textures/dog_1.jpg", 3));

        auto material_02 = std::make_shared<PhongMaterial>();
        material_02->setShiness(32.0);
        material_02->setDiffuse(std::make_shared<Texture>("assets/textures/land.jpg", 3));

        /* 3. 生成mesh*/
        auto mesh_01 = std::make_shared<Mesh>(geometry, material_01);
        auto mesh_02 = std::make_shared<Mesh>(geometry, material_02);
        mesh_02->setPosition(glm::vec3(4.0f, 0.0f, 0.0f));
        meshes.push_back(mesh_01);
        meshes.push_back(mesh_02);

        /* 4. 创建light */
        dirLight = std::make_shared<DirectionalLight>();
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

void doTransform()
{
        meshes[1]->rotateX(1.0f);
        meshes[1]->rotateY(10.0f);
        // transform = glm::rotate(transform, 0.03f, glm::vec3(0.0f, 1.0f, 1.0f));
}

void render()
{
        renderer->render(meshes, camera, dirLight, ambLight);
}

