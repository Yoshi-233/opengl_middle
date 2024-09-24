//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "../../include/core.h"
#include "../../include/scene.h"
#include <vector>
#include "../../include/mesh.h"
#include "../../include/shader.hpp"
#include "../../light/include/directionalLight.h"
#include "../../light/include/pointLight.h"
#include "../../light/include/spotLight.h"
#include "../../light/include/ambientLight.h"
#include "../../materials/include/material.h"
#include "../../materials/include/phongMaterial.h"
#include "../../../application/camera/include/camera.h"


class Renderer {
public:
        Renderer();

        ~Renderer();

        // 聚光灯 + 点光源 + 平行光
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<DirectionalLight> &directionalLight,
                    const std::vector<std::shared_ptr<PointLight>> &pointLights,
                    const std::shared_ptr<SpotLight> &spotLight,
                    const std::shared_ptr<AmbientLight> &ambientLight);

        // 点光源
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<PointLight> &pointLight,
                    const std::shared_ptr<AmbientLight> &ambientLight);

        // 平行光
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<DirectionalLight> &directionalLight,
                    const std::shared_ptr<AmbientLight> &ambientLight);


        void renderObject(const std::shared_ptr<Object> &object,
                          const std::shared_ptr<Camera> &camera,
                          const std::shared_ptr<DirectionalLight> &directionalLight,
                          const std::vector<std::shared_ptr<PointLight>> &pointLights,
                          const std::shared_ptr<SpotLight> &spotLight,
                          const std::shared_ptr<AmbientLight> &ambientLight);

public:
        static void setClearColor(const glm::vec3 &color);

private:
        // 渲染内部调用， 根据类型不同挑选
        std::shared_ptr<Shader> pickShader(MaterialType type);

private:
        static inline void processDirectionLight(const std::shared_ptr<Shader> &shaderPtr,
                                                 const std::shared_ptr<DirectionalLight> &directionalLight);

        static inline void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::shared_ptr<PointLight> &pointLight);

        static inline void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::vector<std::shared_ptr<PointLight>> &pointLights);

        static inline void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::shared_ptr<SpotLight> &spotLight);

        static inline void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::vector<std::shared_ptr<SpotLight>> &spotLights);

        static inline void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                               PhongMaterial *phongMaterial);

        static inline void processCommonInfo(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::shared_ptr<Camera> &camera,
                                             const std::shared_ptr<Mesh> &mesh);

        static inline void processBasicShiness(const std::shared_ptr<Shader> &shaderPtr,
                                               PhongMaterial *phongMaterial,
                                               const std::shared_ptr<AmbientLight> &ambientLight);

        static inline void setGLStatus(const std::shared_ptr<Material> &material);

private:
        // 生成多种shader, 根据材质不同选择
        std::shared_ptr<Shader> mPhoneShader{nullptr};
        std::shared_ptr<Shader> mWhiteShader{nullptr};
};

#endif //RENDERER_RENDERER_H
