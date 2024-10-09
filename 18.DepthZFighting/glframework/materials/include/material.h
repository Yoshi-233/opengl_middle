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

public:
        /* 深度检测相关 */
        bool mDepthTest{true};
        GLenum mDepthFunc{GL_LESS};
        bool mDepthWrite{true};

public:
        /* PolygonOffset */
        bool mPolygonOffset{false};
        unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
        float mFactor{0.0f};
        float mUnit{0.0f};
};

#endif //RENDERER_MATERIAL_H
