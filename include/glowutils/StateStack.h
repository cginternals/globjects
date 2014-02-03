#pragma once

#include <vector>

#include <GL/glew.h>

#include <glow/Referenced.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

#include <glow/State.h>
#include <glow/Capability.h>
#include <glow/StateSetting.h>
/*namespace glow {
class State;
class Capability;
}*/

namespace glowutils {

class GLOWUTILS_API StateStack : public glow::StateInterface, public glow::Referenced
{
public:
    StateStack();

    void push();
    void pop();

    virtual void enable(GLenum capability) override;
    virtual void disable(GLenum capability) override;
    virtual bool isEnabled(GLenum capability) const override;
    virtual void enable(GLenum capability, int index) override;
    virtual void disable(GLenum capability, int index) override;
    virtual bool isEnabled(GLenum capability, int index) const override;
    virtual void set(glow::StateSetting * setting) override;
protected:
    glow::State* m_currentState;
    std::vector<glow::ref_ptr<glow::State>> m_stack;

    void undoState(glow::State * state);
    glow::Capability * findCapability(GLenum capability);
    glow::StateSetting * findSetting(const glow::StateSettingType & type);
};

} // namespace glowutils
