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
    if (m_stack.size() <= 1) // should not happen, remove
    {
        glow::warning() << "Can't pop default state";
        return;
    }

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
            glow::warning() << "Could not undo capability " << glow::enumName(oldCap->capability());
        }
    }

    // TODO: same for CapabilitySettings
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

void StateStack::blendFunc(GLenum sFactor, GLenum dFactor)
{
    m_currentState->blendFunc(sFactor, dFactor);
}

void StateStack::logicOp(GLenum opcode)
{
    m_currentState->logicOp(opcode);
}

void StateStack::cullFace(GLenum mode)
{
    m_currentState->cullFace(mode);
}

void StateStack::depthFunc(GLenum func)
{
    m_currentState->depthFunc(func);
}

void StateStack::depthRange(GLdouble nearVal, GLdouble farVal)
{
    m_currentState->depthRange(nearVal, farVal);
}

void StateStack::depthRange(GLfloat nearVal, GLfloat farVal)
{
    m_currentState->depthRange(nearVal, farVal);
}

void StateStack::pointSize(GLfloat size)
{
    m_currentState->pointSize(size);
}

void StateStack::polygonMode(GLenum face, GLenum mode)
{
    m_currentState->polygonMode(face, mode);
}

void StateStack::polygonOffset(GLfloat factor, GLfloat units)
{
    m_currentState->polygonOffset(factor, units);
}

void StateStack::primitiveRestartIndex(GLuint index)
{
    m_currentState->primitiveRestartIndex(index);
}

void StateStack::sampleCoverage(GLfloat value, GLboolean invert)
{
    m_currentState->sampleCoverage(value, invert);
}

void StateStack::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    m_currentState->scissor(x, y, width, height);
}

void StateStack::stencilFunc(GLenum func, GLint ref, GLuint mask)
{
    m_currentState->stencilFunc(func, ref, mask);
}

void StateStack::stencilOp(GLenum fail, GLenum zFail, GLenum zPass)
{
    m_currentState->stencilOp(fail, zFail, zPass);
}

} // namespace glowutils
