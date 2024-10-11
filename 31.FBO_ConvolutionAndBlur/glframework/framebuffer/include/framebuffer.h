//
// Created by nio on 24-10-11.
//

#ifndef FBO_CLASS_FRAMEBUFFER_H
#define FBO_CLASS_FRAMEBUFFER_H

#include "../../include/core.h"
#include "../../include/texture.h"

class Framebuffer {
public:
        Framebuffer(int width, int height);

        ~Framebuffer();

public:
        int mWidth;
        int mHeight;

        unsigned int mFBO{};
        std::shared_ptr<Texture> mColorAttachment;
        std::shared_ptr<Texture> mDepthStencilAttachment;

};


#endif //FBO_CLASS_FRAMEBUFFER_H
