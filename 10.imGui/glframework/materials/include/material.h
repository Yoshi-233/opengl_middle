//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "../../include/core.h"

enum class MaterialType {
        PhongMaterial,
        WhiteMaterial
};

class Material {
public:
        Material();
        ~Material();
public:
        MaterialType mType;
};

#endif //RENDERER_MATERIAL_H
