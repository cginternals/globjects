#include <glow/State.h>

#include <glow/global.h>
#include <glow/Capability.h>

namespace glow {

void StateInterface::setEnabled(GLenum capability, bool enabled)
{
    enabled ? enable(capability) : disable(capability);
}

void StateInterface::setEnabled(GLenum capability, int index, bool enabled)
{
    enabled ? enable(capability) : disable(capability, index);
}

void StateInterface::blendFunc(GLenum sFactor, GLenum dFactor)
{
    set(new StateSetting(glBlendFunc, sFactor, dFactor));
}

void StateInterface::logicOp(GLenum opcode)
{
    set(new StateSetting(glLogicOp, opcode));
}

void StateInterface::cullFace(GLenum mode)
{
    set(new StateSetting(glCullFace, mode));
}

void StateInterface::depthFunc(GLenum func)
{
    set(new StateSetting(glDepthFunc, func));
}

void StateInterface::depthRange(GLdouble nearVal, GLdouble farVal)
{
    set(new StateSetting(glDepthRange, nearVal, farVal));
}

void StateInterface::depthRange(GLfloat nearVal, GLfloat farVal)
{
    set(new StateSetting(glDepthRangef, nearVal, farVal));
}

void StateInterface::pointSize(GLfloat size)
{
    set(new StateSetting(glPointSize, size));
}

void StateInterface::polygonMode(GLenum face, GLenum mode)
{
    auto setting = new StateSetting(glPolygonMode, face, mode);
    setting->type().specializeType(face);
    set(setting);
}

void StateInterface::polygonOffset(GLfloat factor, GLfloat units)
{
    set(new StateSetting(glPolygonOffset, factor, units));
}

void StateInterface::primitiveRestartIndex(GLuint index)
{
    set(new StateSetting(glPrimitiveRestartIndex, index));
}

void StateInterface::sampleCoverage(GLfloat value, GLboolean invert)
{
    set(new StateSetting(glSampleCoverage, value, invert));
}

void StateInterface::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    set(new StateSetting(glScissor, x, y, width, height));
}

void StateInterface::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
    set(new StateSetting(glStencilFunc, func, ref, mask));
}

void StateInterface::stencilOp(GLenum fail, GLenum zFail, GLenum zPass)
{
    set(new StateSetting(glStencilOp, fail, zFail, zPass));
}

//============================================================


State::State(Mode mode)
: m_mode(mode)
{
}

State::~State()
{
    for (const auto& capability : m_capabilities)
    {
        delete capability.second;
    }
    for (const auto& setting : m_settings)
    {
        delete setting.second;
    }
}

State* State::currentState()
{
    State* state = new State(DeferredMode);
    /*
        not handled (yet):
            GL_DEBUG_OUTPUT
            GL_DEBUG_OUTPUT_SYNCHRONOUS
            GL_CLIP_DISTANCEi
    */
    static std::vector<GLenum> capabilities = {
        GL_BLEND,
        GL_DEPTH_CLAMP,
        GL_DITHER,
        GL_FRAMEBUFFER_SRGB,
        GL_TEXTURE_CUBE_MAP_SEAMLESS,
        GL_COLOR_LOGIC_OP,
        GL_CULL_FACE,
        GL_DEPTH_TEST,
        GL_LINE_SMOOTH,
        GL_PROGRAM_POINT_SIZE,
        GL_POLYGON_SMOOTH,
        GL_POLYGON_OFFSET_FILL,
        GL_POLYGON_OFFSET_LINE,
        GL_POLYGON_OFFSET_POINT,
        GL_SAMPLE_COVERAGE,
        GL_MULTISAMPLE,
        GL_SAMPLE_ALPHA_TO_COVERAGE,
        GL_SAMPLE_ALPHA_TO_ONE,
        GL_SAMPLE_MASK,
        GL_PRIMITIVE_RESTART,
        GL_SCISSOR_TEST,
        GL_STENCIL_TEST,
    };

    for (GLenum capability : capabilities)
    {
        state->setToCurrent(capability);
    }

    state->logicOp(getInteger(GL_LOGIC_OP_MODE));
    state->cullFace(getInteger(GL_CULL_FACE_MODE));
    state->depthFunc(getInteger(GL_DEPTH_FUNC));
    auto depthRangeF = getFloats<2>(GL_DEPTH_RANGE);
    state->depthRange(depthRangeF[0], depthRangeF[1]);
    state->pointSize(getFloat(GL_POINT_SIZE));
    state->polygonMode(GL_FRONT_AND_BACK, getInteger(GL_POLYGON_MODE)); // documentation wrong?
    state->polygonOffset(getFloat(GL_POLYGON_OFFSET_FACTOR), getFloat(GL_POLYGON_OFFSET_UNITS));
    state->sampleCoverage(getFloat(GL_SAMPLE_COVERAGE_VALUE), getBoolean(GL_SAMPLE_COVERAGE_INVERT));
    state->primitiveRestartIndex(getInteger(GL_PRIMITIVE_RESTART_INDEX));
    state->setToCurrent(GL_SCISSOR_TEST);
    auto box = getIntegers<4>(GL_SCISSOR_BOX);
    state->scissor(box[0], box[1], box[2], box[3]);
    state->stencilFunc(getInteger(GL_STENCIL_FUNC), getInteger(GL_STENCIL_REF), getInteger(GL_STENCIL_VALUE_MASK));
    state->stencilOp(getInteger(GL_STENCIL_FAIL), getInteger(GL_STENCIL_PASS_DEPTH_FAIL), getInteger(GL_STENCIL_PASS_DEPTH_PASS));

    return state;
}

void State::setToCurrent(GLenum capability)
{
    setEnabled(capability, glow::isEnabled(capability));
}

void State::enable(GLenum capability)
{
    Capability* cap = getCapability(capability);
    cap->enable();
    if (m_mode == ImmediateMode)
        cap->apply();
}

void State::disable(GLenum capability)
{
    Capability* cap = getCapability(capability);
    cap->disable();
    if (m_mode == ImmediateMode)
        cap->apply();
}

bool State::isEnabled(GLenum capability) const
{
    if (m_capabilities.find(capability) == m_capabilities.end())
        return false;

    return m_capabilities.at(capability)->isEnabled();
}

void State::enable(GLenum capability, int index)
{   
    Capability* cap = getCapability(capability);
    cap->enable(index);
    if (m_mode == ImmediateMode)
        cap->apply();
}

void State::disable(GLenum capability, int index)
{
    Capability* cap = getCapability(capability);
    cap->disable(index);
    if (m_mode == ImmediateMode)
        cap->apply();
}

bool State::isEnabled(GLenum capability, int index) const
{
    if (m_capabilities.find(capability) == m_capabilities.end())
        return false;

    return m_capabilities.at(capability)->isEnabled(index);
}

void State::setMode(Mode mode)
{
    m_mode = mode;
}

State::Mode State::mode() const
{
    return m_mode;
}

void State::apply()
{
    for (const auto& capability : m_capabilities)
    {
        capability.second->apply();
    }
    for (const auto& setting : m_settings)
    {
        setting.second->apply();
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

    for (const auto& capability : m_capabilities)
    {
        caps.push_back(capability.second);
    }

    return caps;
}

Capability* State::capability(GLenum capability)
{
    auto it = m_capabilities.find(capability);
    if (it == m_capabilities.end())
        return nullptr;

    return it->second;
}

std::vector<StateSetting*> State::settings() const
{
    std::vector<StateSetting*> settings;

    for (const auto& setting : m_settings)
    {
        settings.push_back(setting.second);
    }

    return settings;
}

StateSetting * State::setting(const StateSettingType & type)
{
    auto it = m_settings.find(type);
    if (it == m_settings.end())
        return nullptr;

    return it->second;
}

void State::set(StateSetting * setting)
{
    auto type = setting->type();
    if (m_settings.find(type) != m_settings.end())
    {
        delete m_settings[type];
        m_settings.erase(type);
    }
    m_settings[type] = setting;

    if (m_mode == ImmediateMode)
        setting->apply();
}

} // namespace glow
