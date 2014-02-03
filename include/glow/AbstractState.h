#pragma once

#include <unordered_map>
#include <vector>

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

    void blendFunc(GLenum sFactor, GLenum dFactor);
    void logicOp(GLenum opcode);
    void cullFace(GLenum mode);
    void depthFunc(GLenum func);
    void depthRange(GLdouble nearVal, GLdouble farVal);
    void depthRange(GLfloat nearVal, GLfloat farVal);
    void pointSize(GLfloat size);
    void polygonMode(GLenum face, GLenum mode);
    void polygonOffset(GLfloat factor, GLfloat units);
    void primitiveRestartIndex(GLuint index);
    void sampleCoverage(GLfloat value, GLboolean invert);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void stencilFunc(GLenum func, GLint ref, GLuint mask);
    void stencilOp(GLenum fail, GLenum zFail, GLenum zPass);

    virtual void set(StateSetting * setting) = 0;

    template <typename... Arguments>
    void set(void (*function)(Arguments...), Arguments... arguments);
};

} // namespace glow


#include <glow/AbstractState.hpp>
