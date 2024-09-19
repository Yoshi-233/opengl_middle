//
// Created by nio on 24-9-19.
//
#include "mesh.h"

Mesh::Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material)
{
        this->mGeometry = geometry;
        this->mMaterial = material;
}

Mesh::~Mesh()
{

}
