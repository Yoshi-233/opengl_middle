//
// Created by Shaozheming on 2024/9/16.
//

#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include "core.h"
#include <memory>

class Geometry {
public:
        Geometry();

        ~Geometry();

        static std::shared_ptr<Geometry> createBox(float size);

        static std::shared_ptr<Geometry> createSphere(float radius);

        [[nodiscard]] GLuint getVao() const
        { return mVao; };

        [[nodiscard]] GLuint getPosVbo() const
        { return mPosVbo; };

        [[nodiscard]] GLuint getUvVbo() const
        { return mUvVbo; };

        [[nodiscard]] GLuint getEbo() const
        { return mEbo; };

        [[nodiscard]] int getIndicesCount() const
        { return mIndicesCount; };

public:
        GLuint mVao{0};
        GLuint mPosVbo{0};
        GLuint mUvVbo{0};
        GLuint mEbo{0};

        int mIndicesCount{0};

};

#endif //GEOMETRY_GEOMETRY_H
