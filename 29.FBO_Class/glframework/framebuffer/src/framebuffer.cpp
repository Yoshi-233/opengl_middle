//
// Created by nio on 24-10-11.
//

#include "framebuffer.h"

Framebuffer::Framebuffer(int width, int height)
{
        this->mWidth = width;
        this->mHeight = height;
        /* 1.生成FBO对象并且绑定 */
        glGenFramebuffers(1, &this->mFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, this->mFBO);

        /* 2.创建颜色附件，加入fbo */
        this->mColorAttachment
                = Texture::createColorAttachment(0, this->mWidth, this->mHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               this->mColorAttachment->getTextureID(), 0);


        /* 3.创建深度附件，加入fbo */
        this->mDepthStencilAttachment
                = Texture::createDepthStencilAttachment(0, this->mWidth, this->mHeight);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                               this->mDepthStencilAttachment->getTextureID(), 0);

        /* 4.检查fbo是否正确生成 */
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                ERROR("Framebuffer not complete!");
        }

        /* 5.解绑fbo */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
        if (this->mFBO) {
                glDeleteFramebuffers(1, &this->mFBO);
        }
}
