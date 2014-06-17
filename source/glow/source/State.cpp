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
        gl::GL_BLEND,
        gl::GL_COLOR_LOGIC_OP,
        gl::GL_CULL_FACE,
        gl::GL_DEPTH_CLAMP,
        gl::GL_DEPTH_TEST,
        gl::GL_DITHER,
        gl::GL_FRAMEBUFFER_SRGB,
        gl::GL_LINE_SMOOTH,
        gl::GL_MULTISAMPLE,
        gl::GL_POLYGON_OFFSET_FILL,
        gl::GL_POLYGON_OFFSET_LINE,
        gl::GL_POLYGON_OFFSET_POINT,
        gl::GL_POLYGON_SMOOTH,
        gl::GL_PROGRAM_POINT_SIZE,
        gl::GL_RASTERIZER_DISCARD,
        gl::GL_SAMPLE_ALPHA_TO_COVERAGE,
        gl::GL_SAMPLE_ALPHA_TO_ONE,
        gl::GL_SAMPLE_COVERAGE,
        gl::GL_SAMPLE_MASK,
        gl::GL_SCISSOR_TEST,
        gl::GL_STENCIL_TEST
    };

    if (glow::version() >= Version(3, 1))
    {
        capabilities.push_back(gl::GL_PRIMITIVE_RESTART);
        if (hasExtension(gl::Extension::ARB_ES3_compatibility))
        {
            capabilities.push_back(gl::GL_PRIMITIVE_RESTART_FIXED_INDEX);
        }
        state->primitiveRestartIndex(getInteger(gl::GL_PRIMITIVE_RESTART_INDEX));

        if (hasExtension(gl::Extension::ARB_sample_shading))
        {
            capabilities.push_back(gl::GL_SAMPLE_SHADING);
        }
        if (hasExtension(gl::Extension::ARB_seamless_cube_map))
        {
            capabilities.push_back(gl::GL_TEXTURE_CUBE_MAP_SEAMLESS);
        }
        if (hasExtension(gl::Extension::ARB_provoking_vertex))
        {
            state->provokingVertex(getEnum(gl::GL_PROVOKING_VERTEX));
        }
        // is this necessary here? should be controlled by glow::debugmessageoutput
        if (hasExtension(gl::Extension::KHR_debug))
        {
            capabilities.push_back(gl::GL_DEBUG_OUTPUT);
            capabilities.push_back(gl::GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
    }

    for (gl::GLenum capability : capabilities)
    {
        state->setEnabled(capability, glow::isEnabled(capability));
    }

    state->blendColor(getFloats<4>(gl::GL_BLEND_COLOR));
    state->blendFuncSeparate(getEnum(gl::GL_BLEND_SRC_RGB), getEnum(gl::GL_BLEND_DST_RGB), getEnum(gl::GL_BLEND_SRC_ALPHA), getEnum(gl::GL_BLEND_DST_ALPHA));
    state->clearColor(getFloats<4>(gl::GL_COLOR_CLEAR_VALUE));
    state->clearDepth(getFloat(gl::GL_DEPTH_CLEAR_VALUE));
    state->clearStencil(getInteger(gl::GL_STENCIL_CLEAR_VALUE));
    state->colorMask(getBooleans<4>(gl::GL_COLOR_WRITEMASK));
    state->cullFace(getEnum(gl::GL_CULL_FACE_MODE));
    state->depthFunc(getEnum(gl::GL_DEPTH_FUNC));
    state->depthRange(getFloats<2>(gl::GL_DEPTH_RANGE));
    state->frontFace(getEnum(gl::GL_FRONT_FACE));
    state->logicOp(getEnum(gl::GL_LOGIC_OP_MODE));
    state->pointParameter(gl::GL_POINT_FADE_THRESHOLD_SIZE, getInteger(gl::GL_POINT_FADE_THRESHOLD_SIZE));
    state->pointParameter(gl::GL_POINT_SPRITE_COORD_ORIGIN, getInteger(gl::GL_POINT_SPRITE_COORD_ORIGIN));
    state->pointSize(getFloat(gl::GL_POINT_SIZE));
    state->polygonMode(gl::GL_FRONT_AND_BACK, getEnum(gl::GL_POLYGON_MODE)); // is it right to only set gl::GL_FRONT_AND_BACK?
    state->polygonOffset(getFloat(gl::GL_POLYGON_OFFSET_FACTOR), getFloat(gl::GL_POLYGON_OFFSET_UNITS));
    state->sampleCoverage(getFloat(gl::GL_SAMPLE_COVERAGE_VALUE), getBoolean(gl::GL_SAMPLE_COVERAGE_INVERT));
    state->scissor(getIntegers<4>(gl::GL_SCISSOR_BOX));
    state->stencilFuncSeparate(gl::GL_FRONT, getEnum(gl::GL_STENCIL_FUNC), getInteger(gl::GL_STENCIL_REF), getInteger(gl::GL_STENCIL_VALUE_MASK));
    state->stencilOpSeparate(gl::GL_FRONT, getEnum(gl::GL_STENCIL_FAIL), getEnum(gl::GL_STENCIL_PASS_DEPTH_FAIL), getEnum(gl::GL_STENCIL_PASS_DEPTH_PASS));
    state->stencilMaskSeparate(gl::GL_FRONT, getInteger(gl::GL_STENCIL_WRITEMASK));
    state->stencilFuncSeparate(gl::GL_BACK, getEnum(gl::GL_STENCIL_BACK_FUNC), getInteger(gl::GL_STENCIL_BACK_REF), getInteger(gl::GL_STENCIL_BACK_VALUE_MASK));
    state->stencilOpSeparate(gl::GL_BACK, getEnum(gl::GL_STENCIL_BACK_FAIL), getEnum(gl::GL_STENCIL_BACK_PASS_DEPTH_FAIL), getEnum(gl::GL_STENCIL_BACK_PASS_DEPTH_PASS));
    state->stencilMaskSeparate(gl::GL_BACK, getInteger(gl::GL_STENCIL_BACK_WRITEMASK));

    // pixel store
    std::vector<gl::GLenum> pixelstoreParameters = {
        gl::GL_PACK_SWAP_BYTES,
        gl::GL_PACK_LSB_FIRST,
        gl::GL_PACK_ROW_LENGTH,
        gl::GL_PACK_IMAGE_HEIGHT,
        gl::GL_PACK_SKIP_PIXELS,
        gl::GL_PACK_SKIP_ROWS,
        gl::GL_PACK_SKIP_IMAGES,
        gl::GL_PACK_ALIGNMENT,
        gl::GL_UNPACK_SWAP_BYTES,
        gl::GL_UNPACK_LSB_FIRST,
        gl::GL_UNPACK_ROW_LENGTH,
        gl::GL_UNPACK_IMAGE_HEIGHT,
        gl::GL_UNPACK_SKIP_PIXELS,
        gl::GL_UNPACK_SKIP_ROWS,
        gl::GL_UNPACK_SKIP_IMAGES,
        gl::GL_UNPACK_ALIGNMENT
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
