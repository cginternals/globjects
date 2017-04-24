
#include <globjects/AbstractState.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/functions-patches.h>
#include <glbinding/gl/enum.h>

#include <globjects/StateSetting.h>

using namespace gl;


namespace globjects 
{


void AbstractState::setEnabled(GLenum capability, const bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void AbstractState::setEnabled(GLenum capability, const int index, const bool enabled)
{
    enabled ? enable(capability) : disable(capability, index);
}

void AbstractState::blendColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    add(StateSetting::create(glBlendColor, red, green, blue, alpha));
}

void AbstractState::blendColor(const std::array<GLfloat, 4> & color)
{
    blendColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::blendFunc(const GLenum sFactor, const GLenum dFactor)
{
    add(StateSetting::create(glBlendFunc, sFactor, dFactor));
}

void AbstractState::blendFuncSeparate(const GLenum srcRGB, const GLenum dstRGB, const GLenum srcAlpha, const GLenum dstAlpha)
{
    add(StateSetting::create(glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha));
}

void AbstractState::clearColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    add(StateSetting::create(glClearColor, red, green, blue, alpha));
}

void AbstractState::clearColor(const std::array<GLfloat, 4> & color)
{
    clearColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::clearDepth(const GLfloat depth)
{
    add(StateSetting::create(glClearDepthf, depth));
}

void AbstractState::clearStencil(const GLint s)
{
    add(StateSetting::create(glClearStencil, s));
}

void AbstractState::colorMask(const GLboolean red, const GLboolean green, const GLboolean blue, const GLboolean alpha)
{
    add(StateSetting::create(glColorMask, red, green, blue, alpha));
}

void AbstractState::colorMask(const std::array<GLboolean, 4> & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void AbstractState::cullFace(const GLenum mode)
{
    add(StateSetting::create(glCullFace, mode));
}

void AbstractState::depthFunc(const GLenum func)
{
    add(StateSetting::create(glDepthFunc, func));
}

void AbstractState::depthMask(const GLboolean flag)
{
    add(StateSetting::create(glDepthMask, flag));
}

void AbstractState::depthRange(const GLdouble nearVal, const GLdouble farVal)
{
    add(StateSetting::create(glDepthRange, nearVal, farVal));
}

void AbstractState::depthRange(const GLfloat nearVal, const GLfloat farVal)
{
    add(StateSetting::create(glDepthRangef, nearVal, farVal));
}

void AbstractState::depthRange(const std::array<GLfloat, 2> & range)
{
    depthRange(range[0], range[1]);
}
    
void AbstractState::frontFace(GLenum winding)
{
    add(StateSetting::create(glFrontFace, winding));
}

void AbstractState::logicOp(const GLenum opcode)
{
    add(StateSetting::create(glLogicOp, opcode));
}

void AbstractState::pixelStore(const GLenum pname, const GLboolean param)
{
    auto setting = StateSetting::create(static_cast<void(*)(GLenum,GLboolean)>(glPixelStorei), pname, param);
    setting->type().specializeType(pname);
    add(std::move(setting));
}

void AbstractState::pixelStore(gl::GLenum pname, const gl::GLint param)
{
    auto setting = StateSetting::create(static_cast<void(*)(GLenum,GLint)>(glPixelStorei), pname, param);
    setting->type().specializeType(pname);
    add(std::move(setting));
}

void AbstractState::pixelStore(gl::GLenum pname, const gl::GLfloat param)
{
    auto setting = StateSetting::create(glPixelStoref, pname, param);
    setting->type().specializeType(pname);
    add(std::move(setting));
}

void AbstractState::pointParameter(const GLenum pname, const GLenum param)
{
    auto setting = StateSetting::create(static_cast<void(*)(GLenum,GLenum)>(glPointParameteri), pname, param);
    setting->type().specializeType(pname);
    add(std::move(setting));
}

void AbstractState::pointSize(const GLfloat size)
{
    add(StateSetting::create(glPointSize, size));
}

void AbstractState::polygonMode(const GLenum face, const GLenum mode)
{
    auto setting = StateSetting::create(glPolygonMode, face, mode);
    setting->type().specializeType(face);
    add(std::move(setting));
}

void AbstractState::polygonOffset(const GLfloat factor, const GLfloat units)
{
    add(StateSetting::create(glPolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(const GLuint index)
{
    add(StateSetting::create(glPrimitiveRestartIndex, index));
}

void AbstractState::provokingVertex(const GLenum provokeMode)
{
    add(StateSetting::create(glProvokingVertex, provokeMode));
}

void AbstractState::sampleCoverage(const GLfloat value, const GLboolean invert)
{
    add(StateSetting::create(glSampleCoverage, value, invert));
}

void AbstractState::scissor(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
{
    add(StateSetting::create(glScissor, x, y, width, height));
}

void AbstractState::scissor(const std::array<GLint, 4> & scissorBox)
{
    scissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void AbstractState::stencilFunc(const GLenum func, const GLint ref, const GLuint mask)
{
    add(StateSetting::create(glStencilFunc, func, ref, mask));
}

void AbstractState::stencilMask(const GLuint mask)
{
    add(StateSetting::create(glStencilMask, mask));
}

void AbstractState::stencilOp(const GLenum stencilFail, const GLenum depthFail, const GLenum depthPass)
{
    add(StateSetting::create(glStencilOp, stencilFail, depthFail, depthPass));
}

void AbstractState::stencilFuncSeparate(const GLenum face, const GLenum func, const GLint ref, const GLuint mask)
{
    auto setting = StateSetting::create(glStencilFuncSeparate, face, func, ref, mask);
    setting->type().specializeType(face);
    add(std::move(setting));
}

void AbstractState::stencilMaskSeparate(const GLenum face, const GLuint mask)
{
    auto setting = StateSetting::create(glStencilMaskSeparate, face, mask);
    setting->type().specializeType(face);
    add(std::move(setting));
}

void AbstractState::stencilOpSeparate(const GLenum face, const GLenum stencilFail, const GLenum depthFail, const GLenum depthPass)
{
    auto setting = StateSetting::create(glStencilOpSeparate, face, stencilFail, depthFail, depthPass);
    setting->type().specializeType(face);
    add(std::move(setting));
}


} // namespace globjects
