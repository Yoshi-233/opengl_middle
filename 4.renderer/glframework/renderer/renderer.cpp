//
// Created by Shaozheming on 2024/9/19.
//

#include "include/renderer.h"

Renderer::Renderer()
{
        this->mPhoneShader = std::make_shared<Shader>("asserts/shaders/phone.vert",
                                                      "asserts/shaders/phone.frag");
}

Renderer::~Renderer()
= default;

void Renderer::render(const std::vector<std::shared_ptr<Mesh>> &meshes, std::shared_ptr<Camera> camera,
                      std::shared_ptr<DirectionalLight> directionalLight, std::shared_ptr<AmbientLight> ambientLight)
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
                // diffuse贴图
                // 设置sampler采样第0号纹理，注意这里默认是0
                shader->setInt("grassSampler", 0);
                shader->setInt("landSampler", 1);
                shader->setInt("noiseSampler", 2);
                shader->setInt("dogSampler", 3);

                shader->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix", camera->getViewMatrix());
                shader->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix", camera->getProjectionMatrix());

                // 光源参数更新
                shader->setVectorFloat("lightDirection", directionalLight->getDirection());
                shader->setVectorFloat("lightColor", directionalLight->getColor());
                shader->setFloat("specularIntensity", directionalLight->getSpecularIntensity());
                shader->setVectorFloat("ambientColor", ambientLight->getColor());
                shader->setVectorFloat("cameraPosition", camera->mPosition);

                /* 3.3 绑定vao材质 */
                // dogTexture->bind();
                // glBindVertexArray(geometry->getVao());

                /* 3.4 绘制 */
                /* 第一次绘制 */
                // glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
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

