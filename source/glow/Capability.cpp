#include <glow/Capability.h>

#include <glow/State.h>
#include <glow/Error.h>
#include <glow/global.h>

namespace glow {

Capability::Capability(GLenum capability)
: m_capability(capability)
, m_enabled(false)
{
}

Capability::Capability(GLenum capability, bool enabled)
: m_capability(capability)
, m_enabled(enabled)
{
}

GLenum Capability::capability() const
{
    return m_capability;
}

void Capability::enable()
{
    m_enabled = true;
}

void Capability::disable()
{
    m_enabled = false;
}

bool Capability::isEnabled() const
{
    return m_enabled;
}

void Capability::enable(int index)
{
    m_indexEnabled[index] = true;
}

void Capability::disable(int index)
{
    m_indexEnabled[index] = false;
}

bool Capability::isEnabled(int index) const
{
    return m_indexEnabled.at(index);
}

void Capability::apply()
{
    if (m_indexEnabled.empty())
    {
        setEnabled(m_capability, m_enabled);
    }
    else
    {
        for (const std::pair<int, bool>& pair : m_indexEnabled)
        {
            setEnabled(m_capability, pair.first, pair.second);
        }
    }
}

namespace capability
{

AbstractCapabilitySetting::AbstractCapabilitySetting(Type type)
: m_type(type)
{
}

AbstractCapabilitySetting::~AbstractCapabilitySetting()
{
}

AbstractCapabilitySetting::Type AbstractCapabilitySetting::type() const
{
    return m_type;
}


BlendFunc::BlendFunc(GLenum sFactor, GLenum dFactor)
: AbstractCapabilitySetting(T_BlendFunc)
, sFactor(sFactor)
, dFactor(dFactor)
{
}

void BlendFunc::apply()
{
    glBlendFunc(sFactor, dFactor);
    CheckGLError();
}

LogicOp::LogicOp(GLenum opCode)
: AbstractCapabilitySetting(T_LogicOp)
, opCode(opCode)
{
}

void LogicOp::apply()
{
    glLogicOp(opCode);
    CheckGLError();
}

CullFace::CullFace(GLenum mode)
: AbstractCapabilitySetting(T_CullFace)
, mode(mode)
{
}

void CullFace::apply()
{
    glCullFace(mode);
    CheckGLError();
}

DepthFunc::DepthFunc(GLenum func)
: AbstractCapabilitySetting(T_DepthFunc)
, func(func)
{
}

void DepthFunc::apply()
{
    glDepthFunc(func);
    CheckGLError();
}

DepthRange::DepthRange(GLdouble nearVal, GLdouble farVal)
: AbstractCapabilitySetting(T_DepthRange)
, nearVal(nearVal)
, farVal(farVal)
, useFloat(false)
{
}

DepthRange::DepthRange(GLfloat nearValF, GLfloat farValF)
: AbstractCapabilitySetting(T_DepthRange)
, nearValF(nearValF)
, farValF(farValF)
, useFloat(true)
{
}

void DepthRange::apply()
{
    if (useFloat)
    {
        glDepthRangef(nearValF, farValF);
        CheckGLError();
    }
    else
    {
        glDepthRange(nearVal, farVal);
        CheckGLError();
    }
}

PointSize::PointSize(GLfloat size)
: AbstractCapabilitySetting(T_PointSize)
, size(size)
{
}

void PointSize::apply()
{
    glPointSize(size);
    CheckGLError();
}

PolygonMode::PolygonMode(GLenum face, GLenum mode)
: AbstractCapabilitySetting(T_PolygonMode)
, face(face)
, mode(mode)
{

}

void PolygonMode::apply()
{
    glPolygonMode(face, mode);
    CheckGLError();
}

PolygonOffset::PolygonOffset(GLfloat factor, GLfloat units)
: AbstractCapabilitySetting(T_PolygonOffset)
, factor(factor)
, units(units)
{
}

void PolygonOffset::apply()
{
    glPolygonOffset(factor, units);
    CheckGLError();
}

PrimitiveRestartIndex::PrimitiveRestartIndex(GLuint index)
: AbstractCapabilitySetting(T_PrimitiveRestartIndex)
, index(index)
{
}

void PrimitiveRestartIndex::apply()
{
    glPrimitiveRestartIndex(index);
    CheckGLError();
}

SampleCoverage::SampleCoverage(GLfloat value, GLboolean invert)
: AbstractCapabilitySetting(T_SampleCoverage)
, value(value)
, invert(invert)
{
}

void SampleCoverage::apply()
{
    glSampleCoverage(value, invert);
    CheckGLError();
}

Scissor::Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
: AbstractCapabilitySetting(T_Scissor)
, x(x)
, y(y)
, width(width)
, height(height)
{
}

void Scissor::apply()
{
    glScissor(x, y, width, height);
    CheckGLError();
}

StencilFunc::StencilFunc(GLenum func, GLint ref, GLuint mask)
: AbstractCapabilitySetting(T_StencilFunc)
, func(func)
, ref(ref)
, mask(mask)
{
}

void StencilFunc::apply()
{
    glStencilFunc(func, ref, mask);
    CheckGLError();
}

StencilOp::StencilOp(GLenum fail, GLenum zFail, GLenum zPass)
: AbstractCapabilitySetting(T_StencilOp)
, fail(fail)
, zFail(zFail)
, zPass(zPass)
{
}

void StencilOp::apply()
{
    glStencilOp(fail, zFail, zPass);
    CheckGLError();
}

} // namespace capability

} // namespace glow
