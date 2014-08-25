#pragma once

#include <vector>



#include <globjects-base/Referenced.h>
#include <globjects-base/ref_ptr.h>

#include <globjects/AbstractState.h>
#include <globjects/StateSetting.h>

#include <globjects-utils/globjects-utils_api.h>

namespace glo {
class State;
class Capability;
}

namespace gloutils {

class GLOBJECTS_UTILS_API StackedState : public glo::AbstractState, public glo::Referenced
{
public:
    StackedState(glo::State * defaultState = nullptr);

    static StackedState * instance();

    void push();
    void pop();

    virtual void enable(gl::GLenum capability) override;
    virtual void disable(gl::GLenum capability) override;
    virtual bool isEnabled(gl::GLenum capability) const override;
    virtual void enable(gl::GLenum capability, int index) override;
    virtual void disable(gl::GLenum capability, int index) override;
    virtual bool isEnabled(gl::GLenum capability, int index) const override;
    virtual void add(glo::StateSetting * setting) override;
protected:
    glo::State* m_currentState;
    std::vector<glo::ref_ptr<glo::State>> m_stack;

    void undoState(glo::State * state);
    glo::Capability * findCapability(gl::GLenum capability);
    glo::StateSetting * findSetting(const glo::StateSettingType & type);
protected:
    static StackedState * s_instance;
};

} // namespace gloutils
