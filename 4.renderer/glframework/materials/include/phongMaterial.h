//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_PHONGMATERIAL_H
#define RENDERER_PHONGMATERIAL_H

#include "material.h"
#include "../../include/texture.h"

class PhongMaterial : public Material {
public:
        PhongMaterial();
        ~PhongMaterial();
        void setDiffuse(std::shared_ptr<Texture> diffuse);
        void setShiness(float shiness);
        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;
        [[nodiscard]] float getShiness() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        float mShiness{1.0f}; // 高光反射用
};

#endif //RENDERER_PHONGMATERIAL_H
