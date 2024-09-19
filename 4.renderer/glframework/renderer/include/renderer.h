//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "../../include/core.h"
#include <vector>
#include "../../include/mesh.h"
#include "../../include/shader.hpp"
#include "../light/include/directionalLight.h"
#include "../light/include/ambientLight.h"
#include "../materials/include/material.h"
#include "../../../application/camera/include/camera.h"


class Renderer {
public:
        Renderer();

        ~Renderer();

        // 每次调用都会渲染一帧
        void render(const std::vector<std::shared_ptr<Mesh>> &meshes,
                    std::shared_ptr<Camera> camera,
                    std::shared_ptr<DirectionalLight> directionalLight,
                    std::shared_ptr<AmbientLight> ambientLight);

private:
        // 渲染内部调用， 根据类型不同挑选
        std::shared_ptr<Shader> pickShader(MaterialType type);
private:
        // 生成多种shader, 根据材质不同选择
        std::shared_ptr<Shader> mPhoneShader{nullptr};
};

#endif //RENDERER_RENDERER_H
