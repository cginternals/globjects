#pragma once

#include <vector>

#include <GL/glew.h>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>
#include <glow/AbstractState.h>
#include <glow/StateSetting.h>

#include <glowutils/glowutils.h>

namespace glow {
class State;
class Capability;
}

namespace glowutils {

class GLOWUTILS_API StackedState : public glow::AbstractState, public glow::Referenced
{
public:
    StackedState(glow::State * defaultState = nullptr);

    static StackedState * instance();

    void push();
    void pop();

    virtual void enable(GLenum capability) override;
    virtual void disable(GLenum capability) override;
    virtual bool isEnabled(GLenum capability) const override;
    virtual void enable(GLenum capability, int index) override;
    virtual void disable(GLenum capability, int index) override;
    virtual bool isEnabled(GLenum capability, int index) const override;
    virtual void add(glow::StateSetting * setting) override;
protected:
    glow::State* m_currentState;
    std::vector<glow::ref_ptr<glow::State>> m_stack;

    void undoState(glow::State * state);
    glow::Capability * findCapability(GLenum capability);
    glow::StateSetting * findSetting(const glow::StateSettingType & type);
protected:
    static StackedState * s_instance;
};

} // namespace glowutils
