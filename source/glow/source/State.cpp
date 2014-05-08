#include <glow/State.h>

#include <glbinding/constants.h>
#include <glbinding/Extension.h>

#include <glow/glow.h>
#include <glow/Capability.h>

#include <glow/logging.h>
#include <glow/constants.h>

namespace glow {

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

    std::vector<gl::GLenum> capabilities = {
        gl::BLEND,
        gl::COLOR_LOGIC_OP,
        gl::CULL_FACE,
        gl::DEPTH_CLAMP,
        gl::DEPTH_TEST,
        gl::DITHER,
        gl::FRAMEBUFFER_SRGB,
        gl::LINE_SMOOTH,
        gl::MULTISAMPLE,
        gl::POLYGON_OFFSET_FILL,
        gl::POLYGON_OFFSET_LINE,
        gl::POLYGON_OFFSET_POINT,
        gl::POLYGON_SMOOTH,
        gl::PROGRAM_POINT_SIZE,
        gl::RASTERIZER_DISCARD,
        gl::SAMPLE_ALPHA_TO_COVERAGE,
        gl::SAMPLE_ALPHA_TO_ONE,
        gl::SAMPLE_COVERAGE,
        gl::SAMPLE_MASK,
        gl::SCISSOR_TEST,
        gl::STENCIL_TEST
    };

    if (glow::version() >= Version(3, 1))
    {
        capabilities.push_back(gl::PRIMITIVE_RESTART);
        if (hasExtension(gl::Extension::ARB_ES3_compatibility))
        {
            capabilities.push_back(gl::PRIMITIVE_RESTART_FIXED_INDEX);
        }
        state->primitiveRestartIndex(getInteger(gl::PRIMITIVE_RESTART_INDEX));

        if (hasExtension(gl::Extension::ARB_sample_shading))
        {
            capabilities.push_back(gl::SAMPLE_SHADING);
        }
        if (hasExtension(gl::Extension::ARB_seamless_cube_map))
        {
            capabilities.push_back(gl::TEXTURE_CUBE_MAP_SEAMLESS);
        }
        if (hasExtension(gl::Extension::ARB_provoking_vertex))
        {
            state->provokingVertex(getEnum(gl::PROVOKING_VERTEX));
        }
        // is this necessary here? should be controlled by glow::debugmessageoutput
        if (hasExtension(gl::Extension::KHR_debug))
        {
            capabilities.push_back(gl::DEBUG_OUTPUT);
            capabilities.push_back(gl::DEBUG_OUTPUT_SYNCHRONOUS);
        }
    }

    for (gl::GLenum capability : capabilities)
    {
        state->setEnabled(capability, glow::isEnabled(capability));
    }

    state->blendColor(getFloats<4>(gl::BLEND_COLOR));
    state->blendFuncSeparate(getEnum(gl::BLEND_SRC_RGB), getEnum(gl::BLEND_DST_RGB), getEnum(gl::BLEND_SRC_ALPHA), getEnum(gl::BLEND_DST_ALPHA));
    state->clearColor(getFloats<4>(gl::COLOR_CLEAR_VALUE));
    state->clearDepth(getFloat(gl::DEPTH_CLEAR_VALUE));
    state->clearStencil(getInteger(gl::STENCIL_CLEAR_VALUE));
    state->colorMask(getBooleans<4>(gl::COLOR_WRITEMASK));
    state->cullFace(getInteger(gl::CULL_FACE_MODE));
    state->depthFunc(getInteger(gl::DEPTH_FUNC));
    state->depthRange(getFloats<2>(gl::DEPTH_RANGE));
    state->frontFace(getEnum(gl::FRONT_FACE));
    state->logicOp(getInteger(gl::LOGIC_OP_MODE));
    state->pointParameter(gl::POINT_FADE_THRESHOLD_SIZE, getEnum(gl::POINT_FADE_THRESHOLD_SIZE));
    state->pointParameter(gl::POINT_SPRITE_COORD_ORIGIN, getEnum(gl::POINT_SPRITE_COORD_ORIGIN));
    state->pointSize(getFloat(gl::POINT_SIZE));
    state->polygonMode(gl::FRONT_AND_BACK, getInteger(gl::POLYGON_MODE)); // is it right to only set gl::FRONT_AND_BACK?
    state->polygonOffset(getFloat(gl::POLYGON_OFFSET_FACTOR), getFloat(gl::POLYGON_OFFSET_UNITS));
    state->sampleCoverage(getFloat(gl::SAMPLE_COVERAGE_VALUE), getBoolean(gl::SAMPLE_COVERAGE_INVERT));
    state->scissor(getIntegers<4>(gl::SCISSOR_BOX));
    state->stencilFuncSeparate(gl::FRONT, getEnum(gl::STENCIL_FUNC), getEnum(gl::STENCIL_REF), getEnum(gl::STENCIL_VALUE_MASK));
    state->stencilOpSeparate(gl::FRONT, getEnum(gl::STENCIL_FAIL), getEnum(gl::STENCIL_PASS_DEPTH_FAIL), getEnum(gl::STENCIL_PASS_DEPTH_PASS));
    state->stencilMaskSeparate(gl::FRONT, getEnum(gl::STENCIL_WRITEMASK));
    state->stencilFuncSeparate(gl::BACK, getEnum(gl::STENCIL_BACK_FUNC), getEnum(gl::STENCIL_BACK_REF), getEnum(gl::STENCIL_BACK_VALUE_MASK));
    state->stencilOpSeparate(gl::BACK, getEnum(gl::STENCIL_BACK_FAIL), getEnum(gl::STENCIL_BACK_PASS_DEPTH_FAIL), getEnum(gl::STENCIL_BACK_PASS_DEPTH_PASS));
    state->stencilMaskSeparate(gl::BACK, getEnum(gl::STENCIL_BACK_WRITEMASK));

    // pixel store
    std::vector<gl::GLenum> pixelstoreParameters = {
        gl::PACK_SWAP_BYTES,
        gl::PACK_LSB_FIRST,
        gl::PACK_ROW_LENGTH,
        gl::PACK_IMAGE_HEIGHT,
        gl::PACK_SKIP_PIXELS,
        gl::PACK_SKIP_ROWS,
        gl::PACK_SKIP_IMAGES,
        gl::PACK_ALIGNMENT,
        gl::UNPACK_SWAP_BYTES,
        gl::UNPACK_LSB_FIRST,
        gl::UNPACK_ROW_LENGTH,
        gl::UNPACK_IMAGE_HEIGHT,
        gl::UNPACK_SKIP_PIXELS,
        gl::UNPACK_SKIP_ROWS,
        gl::UNPACK_SKIP_IMAGES,
        gl::UNPACK_ALIGNMENT
    };
    for (gl::GLenum param : pixelstoreParameters)
    {
        state->pixelStore(param, getInteger(param));
    }

    return state;
}

void State::enable(gl::GLenum capability)
{
    Capability* cap = getCapability(capability);
    cap->enable();
    if (m_mode == ImmediateMode)
        cap->apply();
}

void State::disable(gl::GLenum capability)
{
    Capability* cap = getCapability(capability);
    cap->disable();
    if (m_mode == ImmediateMode)
        cap->apply();
}

bool State::isEnabled(gl::GLenum capability) const
{
    if (m_capabilities.find(capability) == m_capabilities.end())
        return false;

    return m_capabilities.at(capability)->isEnabled();
}

void State::enable(gl::GLenum capability, int index)
{   
    Capability* cap = getCapability(capability);
    cap->enable(index);
    if (m_mode == ImmediateMode)
        cap->apply();
}

void State::disable(gl::GLenum capability, int index)
{
    Capability* cap = getCapability(capability);
    cap->disable(index);
    if (m_mode == ImmediateMode)
        cap->apply();
}

bool State::isEnabled(gl::GLenum capability, int index) const
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

Capability* State::getCapability(gl::GLenum capability)
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

Capability* State::capability(gl::GLenum capability)
{
    auto it = m_capabilities.find(capability);
    if (it == m_capabilities.end())
        return nullptr;

    return it->second;
}

const Capability* State::capability(gl::GLenum capability) const
{
    auto it = m_capabilities.find(capability);
    if (it == m_capabilities.end())
        return nullptr;

    return it->second;
}

std::vector<StateSetting*> State::settings()
{
    std::vector<StateSetting*> settings;

    for (const auto& setting : m_settings)
    {
        settings.push_back(setting.second);
    }

    return settings;
}

std::vector<const StateSetting*> State::settings() const
{
    std::vector<const StateSetting*> settings;

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

const StateSetting * State::setting(const StateSettingType & type) const
{
    auto it = m_settings.find(type);
    if (it == m_settings.end())
        return nullptr;

    return it->second;
}

void State::add(StateSetting * setting)
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
