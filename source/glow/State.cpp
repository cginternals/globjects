#include <glow/State.h>

#include <glow/global.h>
#include <glow/Capability.h>

namespace glow {

State::State()
{
}

State::~State()
{
    for (const std::pair<GLenum, Capability*>& capability : m_capabilities)
    {
        delete capability.second;
    }
    for (const std::pair<unsigned, capability::AbstractCapabilitySetting*>& capabilitySetting : m_capabilitySettings)
    {
        delete capabilitySetting.second;
    }
}

State* State::currentState()
{
    State* state = new State;
    /*
        not handled (yet):
            GL_DEBUG_OUTPUT
            GL_DEBUG_OUTPUT_SYNCHRONOUS
            GL_CLIP_DISTANCEi
    */

    state->setToCurrent(GL_BLEND);
    state->setToCurrent(GL_DEPTH_CLAMP);
    state->setToCurrent(GL_DITHER);
    state->setToCurrent(GL_FRAMEBUFFER_SRGB);
    state->setToCurrent(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    state->setToCurrent(GL_COLOR_LOGIC_OP);
    state->addCapabilitySetting(new capability::LogicOp(getInteger(GL_LOGIC_OP_MODE)));

    state->setToCurrent(GL_CULL_FACE);
    state->addCapabilitySetting(new capability::CullFace(getInteger(GL_CULL_FACE_MODE)));

    state->setToCurrent(GL_DEPTH_TEST);
    state->addCapabilitySetting(new capability::DepthFunc(getInteger(GL_DEPTH_FUNC)));
    auto depthRangeF = getFloats<2>(GL_DEPTH_RANGE);
    state->addCapabilitySetting(new capability::DepthRange(depthRangeF[0], depthRangeF[1]));

    state->setToCurrent(GL_LINE_SMOOTH);

    state->setToCurrent(GL_PROGRAM_POINT_SIZE);
    state->addCapabilitySetting(new capability::PointSize(getFloat(GL_POINT_SIZE)));

    state->setToCurrent(GL_POLYGON_SMOOTH);
    state->setToCurrent(GL_POLYGON_OFFSET_FILL);
    state->setToCurrent(GL_POLYGON_OFFSET_LINE);
    state->setToCurrent(GL_POLYGON_OFFSET_POINT);
    state->addCapabilitySetting(new capability::PolygonMode(GL_FRONT_AND_BACK, getInteger(GL_POLYGON_MODE))); // documentation wrong?
    state->addCapabilitySetting(new capability::PolygonOffset(getFloat(GL_POLYGON_OFFSET_FACTOR), getFloat(GL_POLYGON_OFFSET_UNITS)));

    state->setToCurrent(GL_SAMPLE_COVERAGE);
    state->setToCurrent(GL_MULTISAMPLE);
    state->setToCurrent(GL_SAMPLE_ALPHA_TO_COVERAGE);
    state->setToCurrent(GL_SAMPLE_ALPHA_TO_ONE);
    state->setToCurrent(GL_SAMPLE_MASK);
    state->addCapabilitySetting(new capability::SampleCoverage(getFloat(GL_SAMPLE_COVERAGE_VALUE), getBoolean(GL_SAMPLE_COVERAGE_INVERT)));

    state->setToCurrent(GL_PRIMITIVE_RESTART);
    state->addCapabilitySetting(new capability::PrimitiveRestartIndex(getInteger(GL_PRIMITIVE_RESTART_INDEX)));

    state->setToCurrent(GL_SCISSOR_TEST);
    auto box = getIntegers<4>(GL_SCISSOR_BOX);
    state->addCapabilitySetting(new capability::Scissor(box[0], box[1], box[2], box[3]));

    state->setToCurrent(GL_STENCIL_TEST);
    state->addCapabilitySetting(new capability::StencilFunc(getInteger(GL_STENCIL_FUNC), getInteger(GL_STENCIL_REF), getInteger(GL_STENCIL_VALUE_MASK)));
    state->addCapabilitySetting(new capability::StencilOp(getInteger(GL_STENCIL_FAIL), getInteger(GL_STENCIL_PASS_DEPTH_FAIL), getInteger(GL_STENCIL_PASS_DEPTH_PASS)));

    return state;
}

void State::setToCurrent(GLenum capability)
{
    setEnabled(capability, glow::isEnabled(capability));
}

void State::enable(GLenum capability)
{
    getCapability(capability)->enable();
}

void State::disable(GLenum capability)
{
   getCapability(capability)->disable();
}

bool State::isEnabled(GLenum capability) const
{
    if (m_capabilities.find(capability) == m_capabilities.end())
        return false;

    return m_capabilities.at(capability)->isEnabled();
}

void State::enable(GLenum capability, int index)
{
    getCapability(capability)->enable(index);
}

void State::disable(GLenum capability, int index)
{
    getCapability(capability)->disable(index);
}

bool State::isEnabled(GLenum capability, int index) const
{
    if (m_capabilities.find(capability) == m_capabilities.end())
        return false;

    return m_capabilities.at(capability)->isEnabled(index);
}

void State::setEnabled(GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void State::setEnabled(GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability, index) : disable(capability, index);
}

void State::apply()
{
    for (const std::pair<GLenum, Capability*>& capability : m_capabilities)
    {
        capability.second->apply();
    }
    for (const std::pair<unsigned, capability::AbstractCapabilitySetting*>& capabilitySetting : m_capabilitySettings)
    {
        capabilitySetting.second->apply();
    }
}

void State::addCapability(Capability * capability)
{
    if (m_capabilities.find(capability->capability()) != m_capabilities.end())
    {
        delete m_capabilities[capability->capability()];
    }

    m_capabilities[capability->capability()] = capability;
}

void State::addCapabilitySetting(capability::AbstractCapabilitySetting * capabilitySetting)
{
    if (m_capabilitySettings.find(capabilitySetting->type()) != m_capabilitySettings.end())
    {
        delete m_capabilitySettings[capabilitySetting->type()];
    }

    m_capabilitySettings[capabilitySetting->type()] = capabilitySetting;
}

Capability* State::getCapability(GLenum capability)
{
    if (m_capabilities.find(capability) == m_capabilities.end())
    {
        m_capabilities[capability] = new Capability(capability);
    }

    return m_capabilities[capability];
}

std::vector<Capability*> State::capabilities() const
{
    std::vector<Capability*> caps;

    for (const std::pair<GLenum, Capability*>& capability : m_capabilities)
    {
        caps.push_back(capability.second);
    }

    return caps;
}

// specific parameters

void State::blendFunc(GLenum sFactor, GLenum dFactor)
{
    addCapabilitySetting(new capability::BlendFunc(sFactor, dFactor));
}

void State::logicOp(GLenum opcode)
{
    addCapabilitySetting(new capability::LogicOp(opcode));
}

void State::cullFace(GLenum mode)
{
    addCapabilitySetting(new capability::CullFace(mode));
}

void State::depthFunc(GLenum func)
{
    addCapabilitySetting(new capability::DepthFunc(func));
}

void State::depthRange(GLdouble nearVal, GLdouble farVal)
{
    addCapabilitySetting(new capability::DepthRange(nearVal, farVal));
}

void State::depthRange(GLfloat nearVal, GLfloat farVal)
{
    addCapabilitySetting(new capability::DepthRange(nearVal, farVal));
}

void State::pointSize(GLfloat size)
{
    addCapabilitySetting(new capability::PointSize(size));
}

void State::polygonMode(GLenum face, GLenum mode)
{
    addCapabilitySetting(new capability::PolygonMode(face, mode));
}

void State::polygonOffset(GLfloat factor, GLfloat units)
{
    addCapabilitySetting(new capability::PolygonOffset(factor, units));
}

void State::primitiveRestartIndex(GLuint index)
{
    addCapabilitySetting(new capability::PrimitiveRestartIndex(index));
}

void State::sampleCoverage(GLfloat value, GLboolean invert)
{
    addCapabilitySetting(new capability::SampleCoverage(value, invert));
}

void State::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    addCapabilitySetting(new capability::Scissor(x, y, width, height));
}

void State::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
    addCapabilitySetting(new capability::StencilFunc(func, ref, mask));
}

void State::stencilOp(GLenum fail, GLenum zFail, GLenum zPass)
{
    addCapabilitySetting(new capability::StencilOp(fail, zFail, zPass));
}

} // namespace glow
