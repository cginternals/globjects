#pragma once

#include <vector>



#include <glowbase/Referenced.h>
#include <glowbase/ref_ptr.h>

#include <glow/AbstractState.h>
#include <glow/StateSetting.h>

#include <glowutils/glowutils_api.h>

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

    virtual void enable(gl::GLenum capability) override;
    virtual void disable(gl::GLenum capability) override;
    virtual bool isEnabled(gl::GLenum capability) const override;
    virtual void enable(gl::GLenum capability, int index) override;
    virtual void disable(gl::GLenum capability, int index) override;
    virtual bool isEnabled(gl::GLenum capability, int index) const override;
    virtual void add(glow::StateSetting * setting) override;
protected:
    glow::State* m_currentState;
    std::vector<glow::ref_ptr<glow::State>> m_stack;

    void undoState(glow::State * state);
    glow::Capability * findCapability(gl::GLenum capability);
    glow::StateSetting * findSetting(const glow::StateSettingType & type);
protected:
    static StackedState * s_instance;
};

} // namespace glowutils
