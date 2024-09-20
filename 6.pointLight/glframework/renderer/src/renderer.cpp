//
// Created by Shaozheming on 2024/9/19.
//

#include "renderer.h"
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
                default:
                        ERROR("Unsupported material type: {}", static_cast<int>(type));
                        break;
        }

        return result;
}

void Renderer::render(const std::vector<std::shared_ptr<Mesh>> &meshes, const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<DirectionalLight> &directionalLight,
                      const std::shared_ptr<AmbientLight> &ambientLight)
{
        /* 1. opengl的必要状态机 */
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        /* 2.清理画布 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* 3.遍历mesh绘制 */
        for (const auto &mesh: meshes) {
                auto material = mesh->getMaterial();
                auto geometry = mesh->getGeometry();

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
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();

                                // 高光蒙板
                                shaderPtr->setInt("specularMaskSampler", 1);
                                phongMaterial->getSpecularMask()->bind();

                                shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                                                       mesh->getModelMatrix());
                                shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                                        camera->getViewMatrix());
                                shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                                              camera->getProjectionMatrix());

                                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
                                shaderPtr->setMatrix<decltype(normalMatrix)>("normalMatrix", normalMatrix);

                                // 光源参数更新
                                shaderPtr->setVectorFloat("lightDirection", directionalLight->getDirection());
                                shaderPtr->setVectorFloat("lightColor", directionalLight->getColor());
                                shaderPtr->setFloat("specularIntensity", directionalLight->getSpecularIntensity());
                                shaderPtr->setVectorFloat("ambientColor", ambientLight->getColor());
                                shaderPtr->setVectorFloat("cameraPosition", camera->mPosition);
                                shaderPtr->setFloat("shininess", phongMaterial->getShiness());

                        }
                                break;
                        case MaterialType::WhiteMaterial: {
                                shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                                                       mesh->getModelMatrix());
                                shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                                        camera->getViewMatrix());
                                shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                                              camera->getProjectionMatrix());
                        }
                                break;
                        default:
                                continue;
                };

                /* 3.3 绑定vao材质 */
                glBindVertexArray(geometry->getVao());

                /* 3.4 绘制 */
                /* 第一次绘制 */
                glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
                // 这里最好解绑，这样误操作就不会影响当前vao
                Shader::end();
        }
}

void Renderer::render(const std::vector<std::shared_ptr<Mesh>> &meshes, const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<PointLight> &pointLight, const std::shared_ptr<AmbientLight> &ambientLight)
{
        /* 1. opengl的必要状态机 */
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        /* 2.清理画布 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* 3.遍历mesh绘制 */
        for (const auto &mesh: meshes) {
                auto material = mesh->getMaterial();
                auto geometry = mesh->getGeometry();

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
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();

                                // 高光蒙板
                                shaderPtr->setInt("specularMaskSampler", 1);
                                phongMaterial->getSpecularMask()->bind();

                                shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                                                       mesh->getModelMatrix());
                                shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                                        camera->getViewMatrix());
                                shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                                              camera->getProjectionMatrix());

                                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
                                shaderPtr->setMatrix<decltype(normalMatrix)>("normalMatrix", normalMatrix);

                                // 光源参数更新
                                shaderPtr->setVectorFloat("lightPosition", pointLight->getPosition());
                                shaderPtr->setVectorFloat("lightColor", pointLight->getColor());
                                shaderPtr->setFloat("specularIntensity", pointLight->getSpecularIntensity());
                                shaderPtr->setVectorFloat("ambientColor", ambientLight->getColor());
                                shaderPtr->setVectorFloat("cameraPosition", camera->mPosition);
                                shaderPtr->setFloat("shininess", phongMaterial->getShiness());

                                shaderPtr->setFloat("kc", pointLight->getKc());
                                shaderPtr->setFloat("k1", pointLight->getK1());
                                shaderPtr->setFloat("k2", pointLight->getK2());

                        }
                                break;
                        case MaterialType::WhiteMaterial: {
                                shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                                                       mesh->getModelMatrix());
                                shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                                        camera->getViewMatrix());
                                shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                                              camera->getProjectionMatrix());
                        }
                                break;
                        default:
                                continue;
                };

                /* 3.3 绑定vao材质 */
                glBindVertexArray(geometry->getVao());

                /* 3.4 绘制 */
                /* 第一次绘制 */
                glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
                // 这里最好解绑，这样误操作就不会影响当前vao
                Shader::end();
        }
}

