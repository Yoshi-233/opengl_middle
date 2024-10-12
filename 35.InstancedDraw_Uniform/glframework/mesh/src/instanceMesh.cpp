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
}

InstanceMesh::~InstanceMesh()
{
        if (!this->instanceData)
        {
                delete[] this->instanceData;
        }
}


