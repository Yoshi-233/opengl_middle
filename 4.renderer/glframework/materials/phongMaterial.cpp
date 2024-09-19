//
// Created by nio on 24-9-19.
//

#include "phongMaterial.h"

#include <utility>


PhoneMaterial::PhoneMaterial()
{
        this->mType = MaterialType::PhongMaterial;
}

void PhoneMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhoneMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

std::shared_ptr<Texture> PhoneMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhoneMaterial::getShiness() const
{
        return this->mShiness;
}

PhoneMaterial::~PhoneMaterial()
= default;
