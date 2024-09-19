//
// Created by nio on 24-9-19.
//

#include "phongMaterial.h"

#include <utility>


PhongMaterial::PhongMaterial()
{
        this->mType = MaterialType::PhongMaterial;
}

void PhongMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

std::shared_ptr<Texture> PhongMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongMaterial::getShiness() const
{
        return this->mShiness;
}

PhongMaterial::~PhongMaterial()
= default;
