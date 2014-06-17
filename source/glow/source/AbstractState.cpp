#include <glow/AbstractState.h>

#include <glbinding/functions.h>

#include <glow/StateSetting.h>

namespace glow {

void AbstractState::setEnabled(gl::GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void AbstractState::setEnabled(gl::GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability) : disable(capability, index);
}

void AbstractState::blendColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha)
{
    add(new StateSetting(gl::glBlendColor, red, green, blue, alpha));
}

void AbstractState::blendColor(const std::array<gl::GLfloat, 4> & color)
{
    blendColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::blendFunc(gl::GLenum sFactor, gl::GLenum dFactor)
{
    add(new StateSetting(gl::glBlendFunc, sFactor, dFactor));
}

void AbstractState::blendFuncSeparate(gl::GLenum srcRGB, gl::GLenum dstRGB, gl::GLenum srcAlpha, gl::GLenum dstAlpha)
{
    add(new StateSetting(gl::glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha));
}

void AbstractState::clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha)
{
    add(new StateSetting(gl::glClearColor, red, green, blue, alpha));
}

void AbstractState::clearColor(const std::array<gl::GLfloat, 4> & color)
{
    clearColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::clearDepth(gl::GLfloat depth)
{
    add(new StateSetting(gl::glClearDepthf, depth));
}

void AbstractState::clearStencil(gl::GLint s)
{
    add(new StateSetting(gl::glClearStencil, s));
}

void AbstractState::colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha)
{
    add(new StateSetting(gl::glColorMask, red, green, blue, alpha));
}

void AbstractState::colorMask(const std::array<gl::GLboolean, 4> & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void AbstractState::cullFace(gl::GLenum mode)
{
    add(new StateSetting(gl::glCullFace, mode));
}

void AbstractState::depthFunc(gl::GLenum func)
{
    add(new StateSetting(gl::glDepthFunc, func));
}

void AbstractState::depthMask(gl::GLboolean flag)
{
    add(new StateSetting(gl::glDepthMask, flag));
}

void AbstractState::depthRange(gl::GLdouble nearVal, gl::GLdouble farVal)
{
    add(new StateSetting(gl::glDepthRange, nearVal, farVal));
}

void AbstractState::depthRange(gl::GLfloat nearVal, gl::GLfloat farVal)
{
    add(new StateSetting(gl::glDepthRangef, nearVal, farVal));
}

void AbstractState::depthRange(const std::array<gl::GLfloat, 2> & range)
{
    depthRange(range[0], range[1]);
}
    
    void AbstractState::frontFace(gl::GLenum winding) {
        add(new StateSetting(gl::glFrontFace, winding));
    }

void AbstractState::logicOp(gl::GLenum opcode)
{
    add(new StateSetting(gl::glLogicOp, opcode));
}

void AbstractState::pixelStore(gl::GLenum pname, gl::GLint param)
{
    auto setting = new StateSetting(gl::glPixelStorei, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointParameter(gl::GLenum pname, gl::GLint param)
{
    auto setting = new StateSetting(gl::glPointParameteri, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointSize(gl::GLfloat size)
{
    add(new StateSetting(gl::glPointSize, size));
}

void AbstractState::polygonMode(gl::GLenum face, gl::GLenum mode)
{
    auto setting = new StateSetting(gl::glPolygonMode, face, mode);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::polygonOffset(gl::GLfloat factor, gl::GLfloat units)
{
    add(new StateSetting(gl::glPolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(gl::GLuint index)
{
    add(new StateSetting(gl::glPrimitiveRestartIndex, index));
}

void AbstractState::provokingVertex(gl::GLenum provokeMode)
{
    add(new StateSetting(gl::glProvokingVertex, provokeMode));
}

void AbstractState::sampleCoverage(gl::GLfloat value, gl::GLboolean invert)
{
    add(new StateSetting(gl::glSampleCoverage, value, invert));
}

void AbstractState::scissor(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height)
{
    add(new StateSetting(gl::glScissor, x, y, width, height));
}

void AbstractState::scissor(const std::array<gl::GLint, 4> & scissorBox)
{
    scissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void AbstractState::stencilFunc(gl::GLenum func, gl::GLint ref, gl::GLuint mask)
{
    add(new StateSetting(gl::glStencilFunc, func, ref, mask));
}

void AbstractState::stencilMask(gl::GLuint mask)
{
    add(new StateSetting(gl::glStencilMask, mask));
}

void AbstractState::stencilOp(gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass)
{
    add(new StateSetting(gl::glStencilOp, stencilFail, depthFail, depthPass));
}

void AbstractState::stencilFuncSeparate(gl::GLenum face, gl::GLenum func, gl::GLint ref, gl::GLuint mask)
{
    auto setting = new StateSetting(gl::glStencilFuncSeparate, face, func, ref, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilMaskSeparate(gl::GLenum face, gl::GLuint mask)
{
    auto setting = new StateSetting(gl::glStencilMaskSeparate, face, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilOpSeparate(gl::GLenum face, gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass)
{
    auto setting = new StateSetting(gl::glStencilOpSeparate, face, stencilFail, depthFail, depthPass);
    setting->type().specializeType(face);
    add(setting);
}

} // namespace glow
