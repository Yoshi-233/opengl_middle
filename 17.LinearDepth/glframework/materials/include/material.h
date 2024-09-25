//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "../../include/core.h"

enum class MaterialType {
        PhongMaterial,
        WhiteMaterial,
        DepthMaterial
};

class Material {
public:
        Material();

        ~Material();

public:
        MaterialType mType;

        bool mDepthTest{true};
        GLenum mDepthFunc{GL_LESS};
        bool mDepthWrite{true};
};

#endif //RENDERER_MATERIAL_H
