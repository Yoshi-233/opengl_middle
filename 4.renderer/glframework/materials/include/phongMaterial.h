//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_PHONGMATERIAL_H
#define RENDERER_PHONGMATERIAL_H

#include "material.h"
#include "texture.h"

class PhoneMaterial : public Material {
public:
        PhoneMaterial();
        ~PhoneMaterial();
public:
        Texture *mDiffuse{nullptr};
        float mShiness{1.0f}; // 高光反射用
};

#endif //RENDERER_PHONGMATERIAL_H
