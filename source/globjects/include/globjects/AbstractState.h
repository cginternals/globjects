
#pragma once

#include <array>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>


namespace globjects
{


class StateSetting;

class GLOBJECTS_API AbstractState
{
public:
    virtual void enable(gl::GLenum capability) = 0;
    virtual void disable(gl::GLenum capability) = 0;
    virtual bool isEnabled(gl::GLenum capability) const = 0;
    virtual void enable(gl::GLenum capability, int index) = 0;
    virtual void disable(gl::GLenum capability, int index) = 0;
    virtual bool isEnabled(gl::GLenum capability, int index) const = 0;

    void setEnabled(gl::GLenum capability, bool enabled);
    void setEnabled(gl::GLenum capability, int index, bool enabled);

    void blendColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha);
    void blendColor(const std::array<gl::GLfloat, 4> & color);
    void blendFunc(gl::GLenum sFactor, gl::GLenum dFactor);
    void blendFuncSeparate(gl::GLenum srcRGB, gl::GLenum dstRGB, gl::GLenum srcAlpha, gl::GLenum dstAlpha);
    void clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha);
    void clearColor(const std::array<gl::GLfloat, 4> & color);
    void clearDepth(gl::GLfloat depth);
    void clearStencil(gl::GLint s);
    void colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha);
    void colorMask(const std::array<gl::GLboolean, 4> & mask);
    void cullFace(gl::GLenum mode);
    void depthFunc(gl::GLenum func);
    void depthMask(gl::GLboolean flag);
    void depthRange(gl::GLdouble nearVal, gl::GLdouble farVal);
    void depthRange(gl::GLfloat nearVal, gl::GLfloat farVal);
    void depthRange(const std::array<gl::GLfloat, 2> & range);
    void frontFace(gl::GLenum winding);
    void logicOp(gl::GLenum opcode);
    void pixelStore(gl::GLenum pname, const gl::GLboolean param);
    void pointParameter(gl::GLenum pname, const gl::GLenum param);
    void pointSize(gl::GLfloat size);
    void polygonMode(gl::GLenum face, gl::GLenum mode);
    void polygonOffset(gl::GLfloat factor, gl::GLfloat units);
    void primitiveRestartIndex(gl::GLuint index);
    void provokingVertex(gl::GLenum provokeMode);
    void sampleCoverage(gl::GLfloat value, gl::GLboolean invert);
    void scissor(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height);
    void scissor(const std::array<gl::GLint, 4> & scissorBox);
    void stencilFunc(gl::GLenum func, gl::GLint ref, gl::GLuint mask);
    void stencilMask(gl::GLuint mask);
    void stencilOp(gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass);
    void stencilFuncSeparate(gl::GLenum face, gl::GLenum func, gl::GLint ref, gl::GLuint mask);
    void stencilMaskSeparate(gl::GLenum face, gl::GLuint mask);
    void stencilOpSeparate(gl::GLenum face, gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass);

    virtual void add(StateSetting * setting) = 0;

    template <typename... Arguments>
    void set(void (*function)(Arguments...), Arguments... arguments);
};


} // namespace globjects


#include <globjects/AbstractState.inl>
