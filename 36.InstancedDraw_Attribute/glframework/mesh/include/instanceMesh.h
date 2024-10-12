//
// Created by nio on 24-9-19.
//

#pragma once

#include "mesh.h"

class InstanceMesh : public Mesh {

public:
        InstanceMesh(const std::shared_ptr<Geometry> &geometry,
                     const std::shared_ptr<Material> &material,
                     unsigned int numInstanced);
        ~InstanceMesh() override;
public:
        void updataMatrices();

public:
        unsigned int numInstanced{0};
        glm::mat4* instanceData{};
        unsigned int mMatrixVBO{0};
};

