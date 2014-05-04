

#pragma once

#include <string>
#include <vector>

#include "glerror.h"
#include "texture.h"
#include "renderbuffer.h"

// Don’t create and destroy framebuffer and renderbuffer objects (or any other large data objects for that matter) per frame.
// Try to avoid modifying textures (using glTexImage2D, glTexSubImage2D, glCopyTeximage2D, and so on) that are attachments to framebuffer objects used as rendering targets.
class Framebuffer {
    GLuint fb;
    // active attachments?
    //std::vector<GLenum> attachments;

public:
    Framebuffer() {
        glGenFramebuffers(1, &fb);    
    }
    
    ~Framebuffer() {
        glDeleteFramebuffers(1, &fb);
    }

    void Attach(const Texture& texture, GLenum attachment, GLint level = 0) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.textureObject, level);
        // glFramebufferTextureLayer // 3D
    }

    void Attach(const RenderBuffer& renderBuffer, GLenum attachment) {    
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer.rb);
    }

    void DetachTexture(GLenum attachment) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
    }

    void DetachRenderBuffer(GLenum attachment) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0);
    }

    /*void SetActiveAttachments() {
        SetActiveAttachments(attachments);
    }*/

    void ActiveColorAttachments(const std::vector<GLenum>& activeAttachments) {
        glDrawBuffers(activeAttachments.size(), &activeAttachments[0]);
    }

    void Bind(GLenum target = GL_FRAMEBUFFER) {
        glBindFramebuffer(target, fb);
    }

    void Unbind() {
        BindDefault();
    }

    static void BindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    //glInvalidateFramebuffer, glInvalidateSubFramebuffer, glBlitFramebuffer, glReadBuffer, glDrawBuffer, (glReadPixels)

    void CheckCompleteness();

private:
    static std::string GetFramebufferStatusString(GLenum status);
};