//
// Created by Shaozheming on 2024/9/19.
//

#include "include/light.h"

glm::vec3 Light::getColor() const
{
        return this->mColor;
}

void Light::setColor(const glm::vec3 &color)
{
        this->mColor = color;
}

float Light::getSpecularIntensity() const
{
        return this->mSpecularIntensity;
}

void Light::setSpecularIntensity(float intensity)
{
        this->mSpecularIntensity = intensity;
}
