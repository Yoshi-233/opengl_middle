//
// Created by nio on 24-9-19.
//

#include "object.h"

Object::Object()
= default;

Object::~Object()
= default;

void Object::setPosition(glm::vec3 pos)
{
        this->mPosition = pos;
}

void Object::rotateX(float angle)
{
        this->mAngleX += angle;
}

void Object::rotateY(float angle)
{
        this->mAngleY += angle;
}

void Object::rotateZ(float angle)
{
        this->mAngleZ += angle;
}

void Object::setScale(glm::vec3 scale)
{
        this->mScale = scale;
}

glm::mat4 Object::getModelMatrix() const
{
        // unity下：先做缩放，旋转，平移
        glm::mat4 transform{1.0f};

        transform = glm::scale(transform, this->mScale);

        // unity旋转标准pitch yaw roll
        // 基于transform后的本地x轴
        transform = glm::rotate(transform, glm::radians(this->mAngleX),
                                glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, glm::radians(this->mAngleY),
                                glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, glm::radians(this->mAngleZ),
                                glm::vec3(0.0, 0.0, 1.0));

        // 平移，在世界坐标系下, mPosition是世界坐标系
        transform = glm::translate(glm::mat4{1.0f}, this->mPosition) * transform;

        return transform;
}
