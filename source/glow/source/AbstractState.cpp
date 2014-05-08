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
    add(new StateSetting(gl::BlendColor, red, green, blue, alpha));
}

void AbstractState::blendColor(const std::array<gl::GLfloat, 4> & color)
{
    blendColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::blendFunc(gl::GLenum sFactor, gl::GLenum dFactor)
{
    add(new StateSetting(gl::BlendFunc, sFactor, dFactor));
}

void AbstractState::blendFuncSeparate(gl::GLenum srcRGB, gl::GLenum dstRGB, gl::GLenum srcAlpha, gl::GLenum dstAlpha)
{
    add(new StateSetting(gl::BlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha));
}

void AbstractState::clearColor(gl::GLfloat red, gl::GLfloat green, gl::GLfloat blue, gl::GLfloat alpha)
{
    add(new StateSetting(gl::ClearColor, red, green, blue, alpha));
}

void AbstractState::clearColor(const std::array<gl::GLfloat, 4> & color)
{
    clearColor(color[0], color[1], color[2], color[3]);
}

void AbstractState::clearDepth(gl::GLfloat depth)
{
    add(new StateSetting(gl::ClearDepthf, depth));
}

void AbstractState::clearStencil(gl::GLint s)
{
    add(new StateSetting(gl::ClearStencil, s));
}

void AbstractState::colorMask(gl::GLboolean red, gl::GLboolean green, gl::GLboolean blue, gl::GLboolean alpha)
{
    add(new StateSetting(gl::ColorMask, red, green, blue, alpha));
}

void AbstractState::colorMask(const std::array<gl::GLboolean, 4> & mask)
{
    colorMask(mask[0], mask[1], mask[2], mask[3]);
}

void AbstractState::cullFace(gl::GLenum mode)
{
    add(new StateSetting(gl::CullFace, mode));
}

void AbstractState::depthFunc(gl::GLenum func)
{
    add(new StateSetting(gl::DepthFunc, func));
}

void AbstractState::depthMask(gl::GLboolean flag)
{
    add(new StateSetting(gl::DepthMask, flag));
}

void AbstractState::depthRange(gl::GLdouble nearVal, gl::GLdouble farVal)
{
    add(new StateSetting(gl::DepthRange, nearVal, farVal));
}

void AbstractState::depthRange(gl::GLfloat nearVal, gl::GLfloat farVal)
{
    add(new StateSetting(gl::DepthRangef, nearVal, farVal));
}

void AbstractState::depthRange(const std::array<gl::GLfloat, 2> & range)
{
    depthRange(range[0], range[1]);
}
    
    void AbstractState::frontFace(gl::GLenum winding) {
        add(new StateSetting(gl::FrontFace, winding));
    }

void AbstractState::logicOp(gl::GLenum opcode)
{
    add(new StateSetting(gl::LogicOp, opcode));
}

void AbstractState::pixelStore(gl::GLenum pname, gl::GLint param)
{
    auto setting = new StateSetting(gl::PixelStorei, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointParameter(gl::GLenum pname, gl::GLint param)
{
    auto setting = new StateSetting(gl::PointParameteri, pname, param);
    setting->type().specializeType(pname);
    add(setting);
}

void AbstractState::pointSize(gl::GLfloat size)
{
    add(new StateSetting(gl::PointSize, size));
}

void AbstractState::polygonMode(gl::GLenum face, gl::GLenum mode)
{
    auto setting = new StateSetting(gl::PolygonMode, face, mode);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::polygonOffset(gl::GLfloat factor, gl::GLfloat units)
{
    add(new StateSetting(gl::PolygonOffset, factor, units));
}

void AbstractState::primitiveRestartIndex(gl::GLuint index)
{
    add(new StateSetting(gl::PrimitiveRestartIndex, index));
}

void AbstractState::provokingVertex(gl::GLenum provokeMode)
{
    add(new StateSetting(gl::ProvokingVertex, provokeMode));
}

void AbstractState::sampleCoverage(gl::GLfloat value, gl::GLboolean invert)
{
    add(new StateSetting(gl::SampleCoverage, value, invert));
}

void AbstractState::scissor(gl::GLint x, gl::GLint y, gl::GLsizei width, gl::GLsizei height)
{
    add(new StateSetting(gl::Scissor, x, y, width, height));
}

void AbstractState::scissor(const std::array<gl::GLint, 4> & scissorBox)
{
    scissor(scissorBox[0], scissorBox[1], scissorBox[2], scissorBox[3]);
}

void AbstractState::stencilFunc(gl::GLenum func, gl::GLint ref, gl::GLuint mask)
{
    add(new StateSetting(gl::StencilFunc, func, ref, mask));
}

void AbstractState::stencilMask(gl::GLuint mask)
{
    add(new StateSetting(gl::StencilMask, mask));
}

void AbstractState::stencilOp(gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass)
{
    add(new StateSetting(gl::StencilOp, stencilFail, depthFail, depthPass));
}

void AbstractState::stencilFuncSeparate(gl::GLenum face, gl::GLenum func, gl::GLint ref, gl::GLuint mask)
{
    auto setting = new StateSetting(gl::StencilFuncSeparate, face, func, ref, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilMaskSeparate(gl::GLenum face, gl::GLuint mask)
{
    auto setting = new StateSetting(gl::StencilMaskSeparate, face, mask);
    setting->type().specializeType(face);
    add(setting);
}

void AbstractState::stencilOpSeparate(gl::GLenum face, gl::GLenum stencilFail, gl::GLenum depthFail, gl::GLenum depthPass)
{
    auto setting = new StateSetting(gl::StencilOpSeparate, face, stencilFail, depthFail, depthPass);
    setting->type().specializeType(face);
    add(setting);
}

} // namespace glow
