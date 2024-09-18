#include "glframework/include/core.h"
#include "glframework/include/shader.hpp"
#include "glframework/include/texture.h"
#include "glframework/include/geometry.h"
#include "application/app/Application.h"
#include "common/include/check_err.h"
#include "common/include/common.h"

#include "application/camera/include/camera.h"
#include "application/camera/include/orthographicCamera.h"
#include "application/camera/include/perspectiveCamera.h"
#include "application/camera/include/trackballCameraController.h"
#include "application/camera/include/gameCameraController.h"


extern std::shared_ptr<Geometry> geometry;
extern std::shared_ptr<Texture> grassTexture;
extern std::shared_ptr<Texture> landTexture;
extern std::shared_ptr<Texture> noiseTexture;
extern std::shared_ptr<Texture> dogTexture;
extern std::shared_ptr<Texture> testLandTexture;
extern glm::mat4 transform;

extern std::unique_ptr<Camera> camera;
extern std::shared_ptr<CameraControl> cameraControl;

extern void prepareAll();

extern void setAPPFunctions();

void render()
{
        // 清屏
        GL_CHECK_ERR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        shader->begin();

        // 设置sampler采样第0号纹理，注意这里默认是0
        shader->setInt("grassSampler", 0);
        shader->setInt("landSampler", 1);
        shader->setInt("noiseSampler", 2);
        shader->setInt("dogSampler", 3);
        shader->setMatrix<decltype(transform)>("transform", transform);
        shader->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix", camera->getViewMatrix());
        shader->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix", camera->getProjectionMatrix());

        dogTexture->bind();
        glBindVertexArray(geometry->getVao());

        /* 第一次绘制 */
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
        // 这里最好解绑，这样误操作就不会影响当前vao
        Shader::end();
}

int main()
{
        cpp_init();
        if (!APP.init(800, 600, "LearnOpenGL")) {
                ERROR("Failed to initialize Application");
                return -1;
        }

        setAPPFunctions();

        // opengl渲染的区域
        GL_CHECK_ERR(glViewport(0, 0, 800, 600));
        // 设置清屏颜色
        GL_CHECK_ERR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

        prepareAll();

        while (APP.update()) {
                cameraControl->update();
                render();
                // 绘制代码
        }
        APP.close();
        return 0;
}