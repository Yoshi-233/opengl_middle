//
// Created by nio on 24-9-19.
//
#include "../include/instanceMesh.h"

InstanceMesh::InstanceMesh(const std::shared_ptr<Geometry> &geometry, const std::shared_ptr<Material> &material,
                           const unsigned int numInstanced) : Mesh(geometry, material)
{
        this->mType = ObjectType::INSTANCED_MESH;
        this->numInstanced = numInstanced;

        this->instanceData = new glm::mat4[numInstanced];

        glGenBuffers(1, &this->mMatrixVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numInstanced,
                     this->instanceData, GL_DYNAMIC_DRAW);

        glBindVertexArray(this->mGeometry->getVao());
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);

        for (int i = 0; i < 4; i++) {
                glEnableVertexAttribArray(3 + i);
                glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE,
                                      sizeof(glm::mat4), (void*)(sizeof(float) * 4 * i));

                glVertexAttribDivisor(3 + i, 1); // 实例渲染
        }
        glBindVertexArray(0);
}

InstanceMesh::~InstanceMesh()
{
        if (!this->instanceData) {
                delete[] this->instanceData;
        }
}

void InstanceMesh::updataMatrices()
{
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);

        // 如果使用glBufferData，会重新分配显存, 第二个参数是offset
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->numInstanced, this->instanceData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}


