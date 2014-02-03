#include <glowutils/StateStack.h>

#include <glow/logging.h>
#include <glow/constants.h>
//#include <glow/State.h>

namespace glowutils
{

StateStack::StateStack()
{
    m_stack.push_back(glow::State::currentState());
    push();
}

void StateStack::push()
{
    m_stack.push_back(m_currentState = new glow::State(glow::State::ImmediateMode));
}

void StateStack::pop()
{
    glow::ref_ptr<glow::State> oldState = m_stack.back();
    m_stack.pop_back();
    m_currentState = m_stack.back();

    undoState(oldState);

    if (m_stack.size() == 1)
        push();
}

void StateStack::undoState(glow::State * state)
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

glow::Capability * StateStack::findCapability(GLenum capability)
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

glow::StateSetting * StateStack::findSetting(const glow::StateSettingType & type)
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


void StateStack::enable(GLenum capability)
{
    m_currentState->enable(capability);
}

void StateStack::disable(GLenum capability)
{
    m_currentState->disable(capability);
}

bool StateStack::isEnabled(GLenum capability) const
{
    return m_currentState->isEnabled(capability);
}

void StateStack::enable(GLenum capability, int index)
{
    m_currentState->enable(capability, index);
}

void StateStack::disable(GLenum capability, int index)
{
    m_currentState->disable(capability, index);
}

bool StateStack::isEnabled(GLenum capability, int index) const
{
    return m_currentState->isEnabled(capability, index);
}

void StateStack::set(glow::StateSetting * setting)
{
    m_currentState->set(setting);
}

} // namespace glowutils
