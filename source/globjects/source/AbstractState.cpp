
#include <globjects/AbstractState.h>

#include <glbinding/gl/functions.h>
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
    add(new StateSetting(glBlendColor, red, green, blue, alpha));
}

void AbstractState::blendColor(const std::array<GLfloat, 4> & color)
{
    blendColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::blendFunc(const GLenum sFactor, const GLenum dFactor)
{
    add(new StateSetting(glBlendFunc, sFactor, dFactor));
}

void AbstractState::blendFuncSeparate(const GLenum srcRGB, const GLenum dstRGB, const GLenum srcAlpha, const GLenum dstAlpha)
{
    add(new StateSetting(glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha));
}

void AbstractState::clearColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
    add(new StateSetting(glClearColor, red, green, blue, alpha));
}

void AbstractState::clearColor(const std::array<GLfloat, 4> & color)
{
    clearColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::clearDepth(const GLfloat depth)
{
    add(new StateSetting(glClearDepthf, depth));
}

void AbstractState::clearStencil(const GLint s)
{
    add(new StateSetting(glClearStencil, s));
}

void AbstractState::colorMask(const GLboolean red, const GLboolean green, const GLboolean blue, const GLboolean alpha)
{
    add(new StateSetting(glColorMask, red, green, blue, alpha));
}

void AbstractState::colorMask(const std::array<GLboolean, 4> & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void AbstractState::cullFace(const GLenum mode)
{
    add(new StateSetting(glCullFace, mode));
}

void AbstractState::depthFunc(const GLenum func)
{
    add(new StateSetting(glDepthFunc, func));
}

void AbstractState::depthMask(const GLboolean flag)
{
    add(new StateSetting(glDepthMask, flag));
}

void AbstractState::depthRange(const GLdouble nearVal, const GLdouble farVal)
{
    add(new StateSetting(glDepthRange, nearVal, farVal));
}

void AbstractState::depthRange(const GLfloat nearVal, const GLfloat farVal)
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

void AbstractState::logicOp(const GLenum opcode)
{
    add(new StateSetting(glLogicOp, opcode));
}

void AbstractState::pixelStore(const GLenum pname, const GLint param)
{
    auto setting = new StateSetting(glPixelStorei, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointParameter(const GLenum pname, const GLint param)
{
    auto setting = new StateSetting(glPointParameteri, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointSize(const GLfloat size)
{
    add(new StateSetting(glPointSize, size));
}

void AbstractState::polygonMode(const GLenum face, const GLenum mode)
{
    auto setting = new StateSetting(glPolygonMode, face, mode);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::polygonOffset(const GLfloat factor, const GLfloat units)
{
    add(new StateSetting(glPolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(const GLuint index)
{
    add(new StateSetting(glPrimitiveRestartIndex, index));
}

void AbstractState::provokingVertex(const GLenum provokeMode)
{
    add(new StateSetting(glProvokingVertex, provokeMode));
}

void AbstractState::sampleCoverage(const GLfloat value, const GLboolean invert)
{
    add(new StateSetting(glSampleCoverage, value, invert));
}

void AbstractState::scissor(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
{
    add(new StateSetting(glScissor, x, y, width, height));
}

void AbstractState::scissor(const std::array<GLint, 4> & scissorBox)
{
    scissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void AbstractState::stencilFunc(const GLenum func, const GLint ref, const GLuint mask)
{
    add(new StateSetting(glStencilFunc, func, ref, mask));
}

void AbstractState::stencilMask(const GLuint mask)
{
    add(new StateSetting(glStencilMask, mask));
}

void AbstractState::stencilOp(const GLenum stencilFail, const GLenum depthFail, const GLenum depthPass)
{
    add(new StateSetting(glStencilOp, stencilFail, depthFail, depthPass));
}

void AbstractState::stencilFuncSeparate(const GLenum face, const GLenum func, const GLint ref, const GLuint mask)
{
    auto setting = new StateSetting(glStencilFuncSeparate, face, func, ref, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilMaskSeparate(const GLenum face, const GLuint mask)
{
    auto setting = new StateSetting(glStencilMaskSeparate, face, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilOpSeparate(const GLenum face, const GLenum stencilFail, const GLenum depthFail, const GLenum depthPass)
{
    auto setting = new StateSetting(glStencilOpSeparate, face, stencilFail, depthFail, depthPass);
    setting->type().specializeType(face);
    add(setting);
}

} // namespace globjects
