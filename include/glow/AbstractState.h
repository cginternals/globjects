#pragma once

#include <unordered_map>
#include <vector>
#include <array>

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow
{

class StateSetting;

class GLOW_API AbstractState
{
public:
    virtual void enable(GLenum capability) = 0;
    virtual void disable(GLenum capability) = 0;
    virtual bool isEnabled(GLenum capability) const = 0;
    virtual void enable(GLenum capability, int index) = 0;
    virtual void disable(GLenum capability, int index) = 0;
    virtual bool isEnabled(GLenum capability, int index) const = 0;

    void setEnabled(GLenum capability, bool enabled);
    void setEnabled(GLenum capability, int index, bool enabled);

    void blendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void blendColor(const std::array<GLfloat, 4> & color);
    void blendFunc(GLenum sFactor, GLenum dFactor);
    void blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void clearColor(const std::array<GLfloat, 4> & color);
    void clearDepth(GLfloat depth);
    void clearStencil(GLint s);
    void colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void colorMask(const std::array<GLboolean, 4> & mask);
    void cullFace(GLenum mode);
    void depthFunc(GLenum func);
    void depthMask(GLboolean flag);
    void depthRange(GLdouble nearVal, GLdouble farVal);
    void depthRange(GLfloat nearVal, GLfloat farVal);
    void depthRange(const std::array<GLfloat, 2> & range);
    void frontFace(GLenum winding);
    void logicOp(GLenum opcode);
    void pixelStore(GLenum pname, GLint param);
    void pointParameter(GLenum pname, GLint param);
    void pointSize(GLfloat size);
    void polygonMode(GLenum face, GLenum mode);
    void polygonOffset(GLfloat factor, GLfloat units);
    void primitiveRestartIndex(GLuint index);
    void provokingVertex(GLenum provokeMode);
    void sampleCoverage(GLfloat value, GLboolean invert);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void scissor(const std::array<GLint, 4> & scissorBox);
    void stencilFunc(GLenum func, GLint ref, GLuint mask);
    void stencilMask(GLuint mask);
    void stencilOp(GLenum stencilFail, GLenum depthFail, GLenum depthPass);
    void stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
    void stencilMaskSeparate(GLenum face, GLuint mask);
    void stencilOpSeparate(GLenum face, GLenum stencilFail, GLenum depthFail, GLenum depthPass);

    virtual void add(StateSetting * setting) = 0;

    template <typename... Arguments>
    void set(void (*function)(Arguments...), Arguments... arguments);
};

} // namespace glow


#include <glow/AbstractState.hpp>
