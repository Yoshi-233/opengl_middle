//
// Created by nio on 24-9-20.
//

#ifndef POINTLIGHT_POINTLIGHT_H
#define POINTLIGHT_POINTLIGHT_H

#include "light.h"
#include "object.h"

class PointLight : public Light, public Object {
public:
        PointLight() = default;

        ~PointLight() override = default;

        float getKc() const;

        float getK1() const;

        float getK2() const;

        void setK2(float mK2);

        void setK1(float mK1);

        void setKc(float mKc);

        void setParameters(float dist);

protected:
        float mK2{1.0f};
        float mK1{1.0f};
        float mKc{1.0f};

};

#endif //POINTLIGHT_POINTLIGHT_H
