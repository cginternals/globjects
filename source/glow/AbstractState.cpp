#include <glow/AbstractState.h>

#include <glow/StateSetting.h>

namespace glow {

void AbstractState::setEnabled(GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void AbstractState::setEnabled(GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability) : disable(capability, index);
}

void AbstractState::blendFunc(GLenum sFactor, GLenum dFactor)
{
    set(new StateSetting(glBlendFunc, sFactor, dFactor));
}

void AbstractState::logicOp(GLenum opcode)
{
    set(new StateSetting(glLogicOp, opcode));
}

void AbstractState::cullFace(GLenum mode)
{
    set(new StateSetting(glCullFace, mode));
}

void AbstractState::depthFunc(GLenum func)
{
    set(new StateSetting(glDepthFunc, func));
}

void AbstractState::depthRange(GLdouble nearVal, GLdouble farVal)
{
    set(new StateSetting(glDepthRange, nearVal, farVal));
}

void AbstractState::depthRange(GLfloat nearVal, GLfloat farVal)
{
    set(new StateSetting(glDepthRangef, nearVal, farVal));
}

void AbstractState::pointSize(GLfloat size)
{
    set(new StateSetting(glPointSize, size));
}

void AbstractState::polygonMode(GLenum face, GLenum mode)
{
    auto setting = new StateSetting(glPolygonMode, face, mode);
    setting->type().specializeType(face);
    set(setting);
}

void AbstractState::polygonOffset(GLfloat factor, GLfloat units)
{
    set(new StateSetting(glPolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(GLuint index)
{
    set(new StateSetting(glPrimitiveRestartIndex, index));
}

void AbstractState::sampleCoverage(GLfloat value, GLboolean invert)
{
    set(new StateSetting(glSampleCoverage, value, invert));
}

void AbstractState::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    set(new StateSetting(glScissor, x, y, width, height));
}

void AbstractState::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
    set(new StateSetting(glStencilFunc, func, ref, mask));
}

void AbstractState::stencilOp(GLenum fail, GLenum zFail, GLenum zPass)
{
    set(new StateSetting(glStencilOp, fail, zFail, zPass));
}

} // namespace glow
