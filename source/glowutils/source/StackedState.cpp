#include <glowutils/StackedState.h>

#include <glow/logging.h>
#include <glow/constants.h>
#include <glow/State.h>
#include <glow/Capability.h>

namespace glowutils
{

StackedState * StackedState::s_instance = nullptr;

StackedState * StackedState::instance()
{
    if (!s_instance)
    {
        s_instance = new StackedState();
    }

    return s_instance;
}

StackedState::StackedState(glow::State * defaultState)
{
    m_stack.push_back(defaultState ? defaultState : glow::State::currentState());
    push();
}

void StackedState::push()
{
    m_stack.push_back(m_currentState = new glow::State(glow::State::ImmediateMode));
}

void StackedState::pop()
{
    glow::ref_ptr<glow::State> oldState = m_stack.back();
    m_stack.pop_back();
    m_currentState = m_stack.back();

    undoState(oldState);

    if (m_stack.size() == 1)
        push();
}

void StackedState::undoState(glow::State * state)
{
    for (glow::Capability* oldCap : state->capabilities())
    {
        glow::Capability* cap = findCapability(oldCap->capability());
        if (cap)
        {
            cap->apply();
        }
        else
        {
            glow::warning() << "Could not undo capability " << glow::enumName(oldCap->capability()) << ".";
        }
    }

    for (glow::StateSetting* oldSetting : state->settings())
    {
        glow::StateSetting* setting = findSetting(oldSetting->type());
        if (setting)
        {
            setting->apply();
        }
        else
        {
            glow::warning() << "Could not undo setting.";
        }
    }
}

glow::Capability * StackedState::findCapability(GLenum capability)
{
    for (int i = static_cast<int>(m_stack.size()-1); i>=0; --i)
    {
        glow::State* state = m_stack[i];
        glow::Capability* cap = state->capability(capability);
        if (cap)
            return cap;
    }

    return nullptr;
}

glow::StateSetting * StackedState::findSetting(const glow::StateSettingType & type)
{
    for (int i = static_cast<int>(m_stack.size()-1); i>=0; --i)
    {
        glow::State* state = m_stack[i];
        glow::StateSetting* setting = state->setting(type);
        if (setting)
            return setting;
    }

    return nullptr;
}


void StackedState::enable(GLenum capability)
{
    m_currentState->enable(capability);
}

void StackedState::disable(GLenum capability)
{
    m_currentState->disable(capability);
}

bool StackedState::isEnabled(GLenum capability) const
{
    return m_currentState->isEnabled(capability);
}

void StackedState::enable(GLenum capability, int index)
{
    m_currentState->enable(capability, index);
}

void StackedState::disable(GLenum capability, int index)
{
    m_currentState->disable(capability, index);
}

bool StackedState::isEnabled(GLenum capability, int index) const
{
    return m_currentState->isEnabled(capability, index);
}

void StackedState::add(glow::StateSetting * setting)
{
    m_currentState->add(setting);
}

} // namespace glowutils
