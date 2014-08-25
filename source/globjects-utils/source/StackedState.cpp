#include <globjects-utils/StackedState.h>

#include <globjects/logging.h>
#include <globjects/constants.h>
#include <globjects/State.h>
#include <globjects/Capability.h>

namespace gloutils
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

StackedState::StackedState(glo::State * defaultState)
{
    m_stack.push_back(defaultState ? defaultState : glo::State::currentState());
    push();
}

void StackedState::push()
{
    m_stack.push_back(m_currentState = new glo::State(glo::State::ImmediateMode));
}

void StackedState::pop()
{
    glo::ref_ptr<glo::State> oldState = m_stack.back();
    m_stack.pop_back();
    m_currentState = m_stack.back();

    undoState(oldState);

    if (m_stack.size() == 1)
        push();
}

void StackedState::undoState(glo::State * state)
{
    for (glo::Capability* oldCap : state->capabilities())
    {
        glo::Capability* cap = findCapability(oldCap->capability());
        if (cap)
        {
            cap->apply();
        }
        else
        {
            glo::warning() << "Could not undo capability " << glo::enumName(oldCap->capability()) << ".";
        }
    }

    for (glo::StateSetting* oldSetting : state->settings())
    {
        glo::StateSetting* setting = findSetting(oldSetting->type());
        if (setting)
        {
            setting->apply();
        }
        else
        {
            glo::warning() << "Could not undo setting.";
        }
    }
}

glo::Capability * StackedState::findCapability(gl::GLenum capability)
{
    for (int i = static_cast<int>(m_stack.size()-1); i>=0; --i)
    {
        glo::State* state = m_stack[i];
        glo::Capability* cap = state->capability(capability);
        if (cap)
            return cap;
    }

    return nullptr;
}

glo::StateSetting * StackedState::findSetting(const glo::StateSettingType & type)
{
    for (int i = static_cast<int>(m_stack.size()-1); i>=0; --i)
    {
        glo::State* state = m_stack[i];
        glo::StateSetting* setting = state->setting(type);
        if (setting)
            return setting;
    }

    return nullptr;
}


void StackedState::enable(gl::GLenum capability)
{
    m_currentState->enable(capability);
}

void StackedState::disable(gl::GLenum capability)
{
    m_currentState->disable(capability);
}

bool StackedState::isEnabled(gl::GLenum capability) const
{
    return m_currentState->isEnabled(capability);
}

void StackedState::enable(gl::GLenum capability, int index)
{
    m_currentState->enable(capability, index);
}

void StackedState::disable(gl::GLenum capability, int index)
{
    m_currentState->disable(capability, index);
}

bool StackedState::isEnabled(gl::GLenum capability, int index) const
{
    return m_currentState->isEnabled(capability, index);
}

void StackedState::add(glo::StateSetting * setting)
{
    m_currentState->add(setting);
}

} // namespace gloutils
