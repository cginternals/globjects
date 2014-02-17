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

void AbstractState::blendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    add(new StateSetting(glBlendColor, red, green, blue, alpha));
}

void AbstractState::blendColor(const std::array<GLfloat, 4> & color)
{
    blendColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::blendFunc(GLenum sFactor, GLenum dFactor)
{
    add(new StateSetting(glBlendFunc, sFactor, dFactor));
}

void AbstractState::blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    add(new StateSetting(glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha));
}

void AbstractState::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    add(new StateSetting(glClearColor, red, green, blue, alpha));
}

void AbstractState::clearColor(const std::array<GLfloat, 4> & color)
{
    clearColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::clearDepth(GLfloat depth)
{
    add(new StateSetting(glClearDepthf, depth));
}

void AbstractState::clearStencil(GLint s)
{
    add(new StateSetting(glClearStencil, s));
}

void AbstractState::colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    add(new StateSetting(glColorMask, red, green, blue, alpha));
}

void AbstractState::colorMask(const std::array<GLboolean, 4> & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void AbstractState::cullFace(GLenum mode)
{
    add(new StateSetting(glCullFace, mode));
}

void AbstractState::depthFunc(GLenum func)
{
    add(new StateSetting(glDepthFunc, func));
}

void AbstractState::depthMask(GLboolean flag)
{
    add(new StateSetting(glDepthMask, flag));
}

void AbstractState::depthRange(GLdouble nearVal, GLdouble farVal)
{
    add(new StateSetting(glDepthRange, nearVal, farVal));
}

void AbstractState::depthRange(GLfloat nearVal, GLfloat farVal)
{
    add(new StateSetting(glDepthRangef, nearVal, farVal));
}

void AbstractState::depthRange(const std::array<GLfloat, 2> & range)
{
    depthRange(range[0], range[1]);
}
    
    void AbstractState::frontFace(GLenum winding) {
        add(new StateSetting(glFrontFace, winding));
    }

void AbstractState::logicOp(GLenum opcode)
{
    add(new StateSetting(glLogicOp, opcode));
}

void AbstractState::pixelStore(GLenum pname, GLint param)
{
    auto setting = new StateSetting(glPixelStorei, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointParameter(GLenum pname, GLint param)
{
    auto setting = new StateSetting(glPointParameteri, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointSize(GLfloat size)
{
    add(new StateSetting(glPointSize, size));
}

void AbstractState::polygonMode(GLenum face, GLenum mode)
{
    auto setting = new StateSetting(glPolygonMode, face, mode);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::polygonOffset(GLfloat factor, GLfloat units)
{
    add(new StateSetting(glPolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(GLuint index)
{
    add(new StateSetting(glPrimitiveRestartIndex, index));
}

void AbstractState::provokingVertex(GLenum provokeMode)
{
    add(new StateSetting(glProvokingVertex, provokeMode));
}

void AbstractState::sampleCoverage(GLfloat value, GLboolean invert)
{
    add(new StateSetting(glSampleCoverage, value, invert));
}

void AbstractState::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    add(new StateSetting(glScissor, x, y, width, height));
}

void AbstractState::scissor(const std::array<GLint, 4> & scissorBox)
{
    scissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void AbstractState::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
    add(new StateSetting(glStencilFunc, func, ref, mask));
}

void AbstractState::stencilMask(GLuint mask)
{
    add(new StateSetting(glStencilMask, mask));
}

void AbstractState::stencilOp(GLenum stencilFail, GLenum depthFail, GLenum depthPass)
{
    add(new StateSetting(glStencilOp, stencilFail, depthFail, depthPass));
}

void AbstractState::stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    auto setting = new StateSetting(glStencilFuncSeparate, face, func, ref, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilMaskSeparate(GLenum face, GLuint mask)
{
    auto setting = new StateSetting(glStencilMaskSeparate, face, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilOpSeparate(GLenum face, GLenum stencilFail, GLenum depthFail, GLenum depthPass)
{
    auto setting = new StateSetting(glStencilOpSeparate, face, stencilFail, depthFail, depthPass);
    setting->type().specializeType(face);
    add(setting);
}

} // namespace glow
