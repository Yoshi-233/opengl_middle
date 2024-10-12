//
// Created by Shaozheming on 2024/9/16.
//

#include "geometry.h"
#include <memory>
#include <vector>

Geometry::Geometry()
= default;

Geometry::Geometry(const std::vector<float> &positions, const std::vector<float> &normals,
                   const std::vector<float> &uvs, const std::vector<unsigned int> &indices)
{
        this->mIndicesCount = static_cast<int>(indices.size());
        // create vbo
        GLuint &posVbo = this->mPosVbo, &uvVbo = this->mUvVbo, &normalVbo = this->mNormalVbo;
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float),
                     positions.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float),
                     uvs.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float),
                     normals.data(), GL_STATIC_DRAW);

        // create ebo
        GLuint &ebo = this->mEbo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);

        /* create vao */
        glGenVertexArrays(1, &this->mVao);
        glBindVertexArray(this->mVao);

        /* bind vbo/ebo */
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindVertexArray(0);

}


Geometry::~Geometry()
{
        if (this->mVao != 0) {
                glDeleteVertexArrays(1, &this->mVao);
        }

        if (this->mPosVbo != 0) {
                glDeleteBuffers(1, &this->mPosVbo);
        }

        if (this->mUvVbo != 0) {
                glDeleteBuffers(1, &this->mUvVbo);
        }

        if (this->mNormalVbo != 0) {
                glDeleteBuffers(1, &this->mNormalVbo);
        }

        if (this->mEbo != 0) {
                glDeleteBuffers(1, &this->mEbo);
        }
}

std::shared_ptr<Geometry> Geometry::createBox(float size)
{
        auto geometry = std::make_shared<Geometry>();
        geometry->mIndicesCount = 36;
        float halfSize = size / 2.0f;
        float positions[] = {
                // Front face
                -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize,
                halfSize, halfSize,
                // Back face
                -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize,
                halfSize, -halfSize, -halfSize,
                // Top face
                -halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize,
                halfSize, -halfSize,
                // Bottom face
                -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize,
                -halfSize, -halfSize, halfSize,
                // Right face
                halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize,
                halfSize, halfSize,
                // Left face
                -halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize,
                -halfSize, halfSize, -halfSize
        };

        float uvs[] = {
                // front
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                // back
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                // top
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                // bottom
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                // right
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                // left
                0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        };

        float normals[] = {
                // front
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                // back
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                // top
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                // bottom
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                // right
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                // left
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
        };

        unsigned int indices[] = {
                0, 1, 2, 2, 3, 0, // front
                4, 5, 6, 6, 7, 4, // back
                8, 9, 10, 10, 11, 8, // top
                12, 13, 14, 14, 15, 12, // bottom
                16, 17, 18, 18, 19, 16, // right
                20, 21, 22, 22, 23, 20 // left
        };

        // create vbo
        GLuint &posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo, &normalVbo = geometry->mNormalVbo;
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glGenBuffers(1, &uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        // create ebo
        GLuint &ebo = geometry->mEbo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /* create vao */
        glGenVertexArrays(1, &geometry->mVao);
        glBindVertexArray(geometry->mVao);

        /* bind vbo/ebo */
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindVertexArray(0);

        return geometry;
}

std::shared_ptr<Geometry> Geometry::createSphere(float radius)
{
        auto geometry = std::make_shared<Geometry>();

        /* 位置, uv, 索引*/
        // 1. 主要变量声明
        std::vector<GLfloat> positions{};
        std::vector<GLfloat> uvs{};
        std::vector<GLfloat> normals{};
        std::vector<GLuint> indices{};

        // 经线纬线
        const int numLatLines = 60;  // 纬线数量
        const int numLongLines = 60; // 经线数量

        // 2. 顶点坐标 uv (纬线在外)
        for (int i = 0; i <= numLatLines; i++) {
                for (int j = 0; j <= numLongLines; j++) {
                        float phi = static_cast<float>(i) * glm::pi<float>() / numLatLines; // 经线角
                        float theta = static_cast<float>(j) * 2.0f * glm::pi<float>() / numLongLines; // 纬线角

                        float x = radius * glm::sin(phi) * glm::cos(theta);
                        float y = radius * glm::cos(phi);
                        float z = radius * glm::sin(phi) * glm::sin(theta);

                        positions.push_back(x);
                        positions.push_back(y);
                        positions.push_back(z);

                        float u = 1 - static_cast<float>(j) / numLongLines;
                        float v = 1 - static_cast<float>(i) / numLatLines;

                        uvs.push_back(u);
                        uvs.push_back(v);

                        // 注意这里的法线没有经过归一化，在shader里进行
                        normals.push_back(x);
                        normals.push_back(y);
                        normals.push_back(z);
                }
        }

        // 3. 顶点索引
        for (int i = 0; i < numLatLines; i++) {
                for (int j = 0; j < numLongLines; j++) {
                        int p1 = i * (numLongLines + 1) + j;
                        int p2 = p1 + numLongLines + 1;
                        int p3 = p1 + 1;
                        int p4 = p2 + 1;
                        indices.push_back(p1);
                        indices.push_back(p2);
                        indices.push_back(p3);

                        indices.push_back(p3);
                        indices.push_back(p2);
                        indices.push_back(p4);
                }
        }

        // create vbo
        GLuint &posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

        // create ebo
        GLuint &ebo = geometry->mEbo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        /* create vao */
        glGenVertexArrays(1, &geometry->mVao);
        glBindVertexArray(geometry->mVao);

        /* bind vbo/ebo */
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindVertexArray(0);

        geometry->mIndicesCount = indices.size();

        return geometry;
}

std::shared_ptr<Geometry> Geometry::createPlane(float width, float height)
{
        auto geometry = std::make_shared<Geometry>();
        geometry->mIndicesCount = 6;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        float positions[] = {
                -halfWidth, -halfHeight, 0.0f,
                halfWidth, -halfHeight, 0.0f,
                halfWidth, halfHeight, 0.0f,
                -halfWidth, halfHeight, 0.0f,
        };

        float normals[] = {
                // front
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f
        };

        float uvs[] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
        };

        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        // create vbo
        GLuint &posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo, &normalVbo = geometry->mNormalVbo;
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glGenBuffers(1, &uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        // create ebo
        GLuint &ebo = geometry->mEbo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /* create vao */
        glGenVertexArrays(1, &geometry->mVao);
        glBindVertexArray(geometry->mVao);

        /* bind vbo/ebo */
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindVertexArray(0);

        return geometry;
}

std::shared_ptr<Geometry> Geometry::createScreenPlane()
{
        auto geometry = std::make_shared<Geometry>();
        geometry->mIndicesCount = 6;

        /* pos uv */
        float positions[] = {
                -1.0f, 1.0f,
                -1.0f, -1.0f,
                1.0f, -1.0f,
                1.0f, 1.0f,
        };

        float uvs[] = {
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
        };

        unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        /* vao vbo */
        GLuint &posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo;
        glGenBuffers(1, &posVbo);
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glGenBuffers(1, &uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

        // create ebo
        GLuint &ebo = geometry->mEbo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        /* create vao */
        glGenVertexArrays(1, &geometry->mVao);
        glBindVertexArray(geometry->mVao);

        /* bind vbo/ebo */
        glBindBuffer(GL_ARRAY_BUFFER, posVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *) 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBindVertexArray(0);

        return geometry;
}

