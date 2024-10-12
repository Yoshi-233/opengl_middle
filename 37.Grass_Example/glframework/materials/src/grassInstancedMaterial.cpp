//
// Created by nio on 24-9-19.
//

#include "../include/grassInstancedMaterial.h"

#include <utility>


GrassInstancedMaterial::GrassInstancedMaterial()
{
        this->mType = MaterialType::GrassInstancedMaterial;
}

void GrassInstancedMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void GrassInstancedMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void GrassInstancedMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> GrassInstancedMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float GrassInstancedMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> GrassInstancedMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


GrassInstancedMaterial::~GrassInstancedMaterial()
= default;
