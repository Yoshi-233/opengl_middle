//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H

#include "core.h"

class Object {
public:
        Object();
        ~Object();

        void setPosition(glm::vec3 pos);
        glm::vec3 getPosition();

        // 增量旋转
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        void setScale(glm::vec3 scale);

        glm::mat4 getModelMatrix() const;

protected:
        glm::vec3 mPosition{0.0f};

        // 个体坐标系 pitch yaw roll
        float mAngleX{0.0f};
        float mAngleY{0.0f};
        float mAngleZ{0.0f};

        glm::vec3 mScale{1.0f};
};

#endif //RENDERER_OBJECT_H
