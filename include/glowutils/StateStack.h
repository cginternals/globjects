#pragma once

#include <vector>

#include <GL/glew.h>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

#include <glow/State.h>
#include <glow/Capability.h>
/*namespace glow {
class State;
class Capability;
}*/

namespace glowutils {

class GLOWUTILS_API StateStack : public glow::StateInterface, public glow::Referenced
{
public:
    StateStack();

    void push();
    void pop();

    virtual void enable(GLenum capability) override;
    virtual void disable(GLenum capability) override;
    virtual bool isEnabled(GLenum capability) const override;
    virtual void enable(GLenum capability, int index) override;
    virtual void disable(GLenum capability, int index) override;
    virtual bool isEnabled(GLenum capability, int index) const override;

    virtual void blendFunc(GLenum sFactor, GLenum dFactor) override;
    virtual void logicOp(GLenum opcode) override;
    virtual void cullFace(GLenum mode) override;
    virtual void depthFunc(GLenum func) override;
    virtual void depthRange(GLdouble nearVal, GLdouble farVal) override;
    virtual void depthRange(GLfloat nearVal, GLfloat farVal) override;
    virtual void pointSize(GLfloat size) override;
    virtual void polygonMode(GLenum face, GLenum mode) override;
    virtual void polygonOffset(GLfloat factor, GLfloat units) override;
    virtual void primitiveRestartIndex(GLuint index) override;
    virtual void sampleCoverage(GLfloat value, GLboolean invert) override;
    virtual void scissor(GLint x, GLint y, GLsizei width, GLsizei height) override;
    virtual void stencilFunc(GLenum func, GLint ref, GLuint mask) override;
    virtual void stencilOp(GLenum fail, GLenum zFail, GLenum zPass) override;
protected:
    glow::State* m_currentState;
    std::vector<glow::ref_ptr<glow::State>> m_stack;

    void undoState(glow::State * state);
    glow::Capability * findCapability(GLenum capability);
};

} // namespace glowutils
