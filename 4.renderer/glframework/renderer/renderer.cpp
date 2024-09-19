//
// Created by Shaozheming on 2024/9/19.
//

#include "include/renderer.h"
#include "../materials/include/phongMaterial.h"
#include <memory>

Renderer::Renderer()
{
        this->mPhoneShader = std::make_shared<Shader>("/home/shao/opengl_middle/4.renderer/assets/shaders/phong.vert",
                                                      "/home/shao/opengl_middle/4.renderer/assets/shaders/phong.frag");
}

Renderer::~Renderer()
= default;

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
                                shaderPtr->setInt("grassSampler", 0);
                                shaderPtr->setInt("landSampler", 1);
                                shaderPtr->setInt("noiseSampler", 2);
                                shaderPtr->setInt("dogSampler", 3);

                                phongMaterial->getDiffuse()->bind();

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
                                break;
                        }
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

std::shared_ptr<Shader> Renderer::pickShader(MaterialType type)
{
        std::shared_ptr<Shader> result = nullptr;

        switch (type) {
                case MaterialType::PhongMaterial:
                        result = this->mPhoneShader;
                        break;
                default:
                        ERROR("Unsupported material type: {}", static_cast<int>(type));
                        break;
        }

        return result;
}

