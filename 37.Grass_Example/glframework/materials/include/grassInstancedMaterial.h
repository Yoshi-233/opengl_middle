//
// Created by nio on 24-9-19.
//

#pragma once

#include "material.h"
#include "../../include/texture.h"

class GrassInstancedMaterial : public Material {
public:
        GrassInstancedMaterial();

        ~GrassInstancedMaterial();

        void setDiffuse(std::shared_ptr<Texture> diffuse);

        void setSpecularMask(std::shared_ptr<Texture> specularMask);

        void setOpacityMask(std::shared_ptr<Texture> opacityMask);

        void setShiness(float shiness);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;

        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;

        [[nodiscard]] std::shared_ptr<Texture> getOpacityMask() const;

        [[nodiscard]] float getShiness() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        std::shared_ptr<Texture> mOpacityMask{nullptr};
        float mShiness{1.0f}; // 高光反射用
};
