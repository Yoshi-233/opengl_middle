//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_MESH_H
#define RENDERER_MESH_H

#include "object.h"
#include "geometry.h"
#include "../materials/include/material.h"

class Mesh : public Object {

public:
        Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material);

        ~Mesh();

        [[nodiscard]] std::shared_ptr<Geometry> getGeometry() const;

        [[nodiscard]] std::shared_ptr<Material> getMaterial() const;

public:
        std::shared_ptr<Geometry> mGeometry{nullptr};
        std::shared_ptr<Material> mMaterial{nullptr};

};

#endif //RENDERER_MESH_H
