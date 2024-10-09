//
// Created by Shaozheming on 2024/9/19.
//

#include "renderer.h"
#include "scene.h"
#include "phongMaterial.h"
#include <memory>

Renderer::Renderer()
{
        auto phongVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phong.vert";
        auto phongFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phong.frag";
        this->mPhoneShader = std::make_shared<Shader>(phongVertexPath.c_str(),
                                                      phongFragmentPath.c_str());

        auto whiteVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/white.vert";
        auto whiteFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/white.frag";
        this->mWhiteShader = std::make_shared<Shader>(whiteVertexPath.c_str(),
                                                      whiteFragmentPath.c_str());

        auto depthVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.vert";
        auto depthFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.frag";
        this->mDepthShader = std::make_shared<Shader>(depthVertexPath.c_str(),
                                                      depthFragmentPath.c_str());

}

Renderer::~Renderer()
= default;

std::shared_ptr<Shader> Renderer::pickShader(MaterialType type)
{
        std::shared_ptr<Shader> result = nullptr;

        switch (type) {
                case MaterialType::PhongMaterial:
                        result = this->mPhoneShader;
                        break;
                case MaterialType::WhiteMaterial:
                        result = this->mWhiteShader;
                        break;
                case MaterialType::DepthMaterial:
                        result = this->mDepthShader;
                        break;
                default:
                        ERROR("Unsupported material type: {}", static_cast<int>(type));
                        break;
        }

        return result;
}

void Renderer::processDirectionLight(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::shared_ptr<DirectionalLight> &directionalLight)
{
        CHECK_POINTER_RETURN_VOID(directionalLight);
        /* 平行光参数 */
        shaderPtr->setVectorFloat("directionalLight.direction",
                                  directionalLight->getDirection());
        shaderPtr->setVectorFloat("directionalLight.color", directionalLight->getColor());
        shaderPtr->setFloat("directionalLight.specularIntensity",
                            directionalLight->getSpecularIntensity());
}

inline void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::shared_ptr<PointLight> &pointLight)
{
        CHECK_POINTER_RETURN_VOID(pointLight);

        shaderPtr->setVectorFloat("pointLight.position",
                                  pointLight->getPosition());
        shaderPtr->setVectorFloat("pointLight.color", pointLight->getColor());
        shaderPtr->setFloat("pointLight.specularIntensity",
                            pointLight->getSpecularIntensity());
        shaderPtr->setFloat("pointLight.kc", pointLight->getKc());
        shaderPtr->setFloat("pointLight.k1", pointLight->getK1());
        shaderPtr->setFloat("pointLight.k2", pointLight->getK2());
}

inline void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::vector<std::shared_ptr<PointLight>> &pointLights)
{
        if (pointLights.empty()) {
                shaderPtr->setFloat("pointLightsFlag", 0);
        } else {
                shaderPtr->setFloat("pointLightsFlag", 1);
        }

        for (int i = 0; i < pointLights.size(); i++) {
                auto baseName = "pointLights[" + std::to_string(i) + "]";

                shaderPtr->setVectorFloat(baseName + ".position",
                                          pointLights[i]->getPosition());
                shaderPtr->setVectorFloat(baseName + ".color", pointLights[i]->getColor());
                shaderPtr->setFloat(baseName + ".specularIntensity",
                                    pointLights[i]->getSpecularIntensity());
                shaderPtr->setFloat(baseName + ".kc", pointLights[i]->getKc());
                shaderPtr->setFloat(baseName + ".k1", pointLights[i]->getK1());
                shaderPtr->setFloat(baseName + ".k2", pointLights[i]->getK2());
        }
}

inline void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                       const std::shared_ptr<SpotLight> &spotLight)
{
        if (spotLight == nullptr) {
                shaderPtr->setFloat("spotLightFlag", 0);
                return;
        } else {
                shaderPtr->setFloat("spotLightFlag", 1);
        }
        CHECK_POINTER_RETURN_VOID(spotLight);
        shaderPtr->setVectorFloat("spotLight.position", spotLight->getPosition());
        shaderPtr->setVectorFloat("spotLight.color", spotLight->getColor());
        shaderPtr->setFloat("spotLight.specularIntensity",
                            spotLight->getSpecularIntensity());
        shaderPtr->setVectorFloat("spotLight.targetDirection",
                                  spotLight->getTargetDirection());
        // 下面是弧度值
        shaderPtr->setFloat("spotLight.innerLine",
                            glm::cos(glm::radians(spotLight->getInnerAngle())));
        shaderPtr->setFloat("spotLight.outerLine",
                            glm::cos(glm::radians(spotLight->getOuterAngle())));
}

inline void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                       const std::vector<std::shared_ptr<SpotLight>> &spotLights)
{
        if (spotLights.empty()) {
                shaderPtr->setFloat("spotLightsFlag", 0);
        } else {
                shaderPtr->setFloat("spotLightsFlag", 1);
        }

        for (int i = 0; i < spotLights.size(); i++) {
                auto baseName = "spotLights[" + std::to_string(i) + "]";

                shaderPtr->setVectorFloat(baseName + ".position", spotLights[i]->getPosition());
                shaderPtr->setVectorFloat(baseName + ".color", spotLights[i]->getColor());
                shaderPtr->setFloat(baseName + ".specularIntensity",
                                    spotLights[i]->getSpecularIntensity());
                shaderPtr->setVectorFloat(baseName + ".targetDirection",
                                          spotLights[i]->getTargetDirection());
                // 下面是弧度值
                shaderPtr->setFloat(baseName + ".innerLine",
                                    glm::cos(glm::radians(spotLights[i]->getInnerAngle())));
                shaderPtr->setFloat(baseName + ".outerLine",
                                    glm::cos(glm::radians(spotLights[i]->getOuterAngle())));
        }
}

void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   PhongMaterial *phongMaterial)
{
        if (phongMaterial->getSpecularMask() != nullptr) {
                // 高光蒙板
                shaderPtr->setFloat("specularMaskFlag", 1.0f);
                shaderPtr->setInt("specularMaskSampler", 1);
                phongMaterial->getSpecularMask()->bind();
        } else {
                shaderPtr->setFloat("specularMaskFlag", 0.0f);
        }
}

void Renderer::processCommonInfo(const std::shared_ptr<Shader> &shaderPtr,
                                 const std::shared_ptr<Camera> &camera,
                                 const std::shared_ptr<Mesh> &mesh)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());

        auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
        shaderPtr->setMatrix<decltype(normalMatrix)>("normalMatrix", normalMatrix);

        // 光源参数更新
        shaderPtr->setVectorFloat("cameraPosition", camera->mPosition);
        shaderPtr->setFloat("near", camera->mNear);
        shaderPtr->setFloat("far", camera->mFar);
}

// 环境光和基础光照
void Renderer::processBasicShiness(const std::shared_ptr<Shader> &shaderPtr,
                                   PhongMaterial *phongMaterial,
                                   const std::shared_ptr<AmbientLight> &ambientLight)
{
        shaderPtr->setVectorFloat("ambientColor", ambientLight->getColor());
        shaderPtr->setFloat("shiness", phongMaterial->getShiness());
}

void Renderer::processWhiteMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                    const std::shared_ptr<Mesh> &mesh,
                                    const std::shared_ptr<Camera> &camera)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());
}

void Renderer::processDepthMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                    const std::shared_ptr<Mesh> &mesh,
                                    const std::shared_ptr<Camera> &camera)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());

        shaderPtr->setFloat("near", camera->mNear);
        shaderPtr->setFloat("far", camera->mFar);
}

void Renderer::renderObject(const std::shared_ptr<Object> &object, const std::shared_ptr<Camera> &camera,
                            const std::shared_ptr<DirectionalLight> &directionalLight,
                            const std::vector<std::shared_ptr<PointLight>> &pointLights,
                            const std::shared_ptr<SpotLight> &spotLight,
                            const std::shared_ptr<AmbientLight> &ambientLight)
{
        if (object->getType() == ObjectType::MESH) {
                auto mesh = std::dynamic_pointer_cast<Mesh>(object);
                auto material = mesh->getMaterial();
                auto geometry = mesh->getGeometry();

                // 设置渲染状态
                Renderer::setGLStatus(material);

                /* 3.1 决定使用哪个shader */
                std::shared_ptr<Shader> shaderPtr = this->pickShader(material->mType);

                /* 3.2 设置shader的uniform变量 */
                shaderPtr->begin();
                // 纹理和纹理单元匹配
                switch (material->mType) {
                        case MaterialType::PhongMaterial: {
                                auto phongMaterial = (PhongMaterial *) material.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();
                                /* 透明度 */
                                shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

                                Renderer::processSpecularMask(shaderPtr, phongMaterial);

                                Renderer::processCommonInfo(shaderPtr, camera, mesh);

                                /* 光源参数更新 */
                                if (ambientLight != nullptr) {
                                        Renderer::processBasicShiness(shaderPtr, phongMaterial, ambientLight);
                                }

                                /* 点光源参数 */
                                Renderer::processPointLight(shaderPtr, pointLights);

                                /* 平行光参数 */
                                if (directionalLight != nullptr) {
                                        Renderer::processDirectionLight(shaderPtr, directionalLight);
                                }

                                /* 聚光灯参数 */
                                Renderer::processSpotLight(shaderPtr, spotLight);
                        }
                                break;
                        case MaterialType::WhiteMaterial: {
                                Renderer::processWhiteMaterial(shaderPtr, mesh, camera);
                        }
                                break;
                        case MaterialType::DepthMaterial: {
                                Renderer::processDepthMaterial(shaderPtr, mesh, camera);
                        }
                                break;
                        default:
                                break;
                };

                /* 3.3 绑定vao材质 */
                glBindVertexArray(geometry->getVao());

                /* 3.4 绘制 */
                /* 第一次绘制 */
                glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
                // 这里最好解绑，这样误操作就不会影响当前vao
                Shader::end();
        }

        /* 2. 对每个子节点都需要调用renderObject */
        auto children = object->getChildren();
        for (auto &child: children) {
                this->renderObject(child, camera, directionalLight, pointLights, spotLight, ambientLight);
        }
}

void Renderer::render(const std::shared_ptr<Scene> &scene,
                      const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<DirectionalLight> &directionalLight,
                      const std::vector<std::shared_ptr<PointLight>> &pointLights,
                      const std::shared_ptr<SpotLight> &spotLight,
                      const std::shared_ptr<AmbientLight> &ambientLight
)
{
        /* 初始化 */
        Renderer::setGLInitStatus();

        /* 从scene根节点绘制 */
        renderObject(std::static_pointer_cast<Object>(scene), camera, directionalLight,
                     pointLights, spotLight, ambientLight);

}

void Renderer::render(const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<DirectionalLight> &directionalLight,
                      const std::shared_ptr<AmbientLight> &ambientLight)
{
        Renderer::setGLInitStatus();

        /* 从scene根节点绘制 */
        auto pointLights = std::vector<std::shared_ptr<PointLight>>{};
        renderObject(std::static_pointer_cast<Object>(scene), camera, directionalLight,
                     pointLights, nullptr, ambientLight);
}

void Renderer::render(const std::shared_ptr<Scene> &scene,
                      const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<PointLight> &pointLight,
                      const std::shared_ptr<AmbientLight> &ambientLight)
{
        Renderer::setGLInitStatus();

        /* 从scene根节点绘制 */
        auto pointLights = std::vector<std::shared_ptr<PointLight>>{};
        pointLights.push_back(pointLight);
        renderObject(std::static_pointer_cast<Object>(scene), camera, nullptr,
                     pointLights, nullptr, ambientLight);
}

void Renderer::setClearColor(const glm::vec3 &color)
{
        glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::setGLInitStatus()
{
        /* 1. opengl的必要状态机, 为默认设置，否则，当最后一个mesh没有写入时，会导致glClear失败 */
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);

        /* 打开模板测试写入 */
        glEnable(GL_STENCIL_TEST);
        // 初始状态下，模板测试通过，模板测试通过但深度测试未通过，模板深度测试都通过，保持不变
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF); // 开启模板测试写入, 0xFF表示全部写入也保证了模板缓冲可以被清理, 0x00表示全部不写入

        /* 默认关闭颜色混合 */
        glDisable(GL_BLEND);

        /* 2.清理画布 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::setGLStatus(const std::shared_ptr<Material> &material)
{
        Renderer::setDepthStatus(material);
        /* PolygonOffset */
        Renderer::setPolygonOffsetStatus(material);
        /* 模板测试 */
        Renderer::setStencilStatus(material);
        /* 混合 */
        Renderer::setBlendStatus(material);

}

void Renderer::setDepthStatus(const std::shared_ptr<Material> &material)
{
        if (material->mDepthTest) {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(material->mDepthFunc);
        } else {
                glDisable(GL_DEPTH_TEST);
        }

        if (material->mDepthWrite) {
                glDepthMask(GL_TRUE);
        } else {
                glDepthMask(GL_FALSE);
        }
}

void Renderer::setPolygonOffsetStatus(const std::shared_ptr<Material> &material)
{
        if (material->mPolygonOffset) {
                glEnable(material->mPolygonOffsetType);
                /*
                 * param1 * 斜率因子 + param2 * 最小精度偏移
                 * 1.第一个参数是斜率因子，当两个比较近的mesh同时旋转时，距离会很小，造成穿模的现象
                 * 此时，近平面影响不大，但远平面较为严重，远平面的偏导数较低（比实际图像的df/dx df/dy低）
                 * 因此斜率因子可以根据这一特性使得较远处的平面偏移更大些
                 * 2. 当两个mesh很近时，出现穿模现象，使用该参数可以让平面稍微后移，精度为float
                 * */
                glPolygonOffset(material->mFactor, material->mUnit);
        } else {
                glDisable(GL_POLYGON_OFFSET_FILL);
                glDisable(GL_POLYGON_OFFSET_LINE);
        }
}

void Renderer::setStencilStatus(const std::shared_ptr<Material> &material)
{
        if (material->mStencilTest) {
                glEnable(GL_STENCIL_TEST);
                // 对于本体, 允许模板测试通过
                glStencilFunc(material->mStencilFunc, (GLint)material->mStencilRef, material->mStencilFuncMask);
                glStencilOp(material->mSFail, material->mZFail, material->mZPass); // 都通过后替换为0x1
                glStencilMask(material->mStencilMask); // 这里开启防止前一个关闭了写入
        } else {
                glDisable(GL_STENCIL_TEST);
        }
}

void Renderer::setBlendStatus(const std::shared_ptr<Material> &material)
{
        if (material->mBlend) {
                glEnable(GL_BLEND);
                // src_color * src_alpha + dst_color * (1 - src_alpha)
                glBlendFunc(material->mSFactor, material->mDFactor);
        } else {
                glDisable(GL_BLEND);
        }
}








