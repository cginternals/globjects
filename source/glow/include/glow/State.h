#pragma once

#include <unordered_map>
#include <vector>

#include <glbinding/types.h>

#include <glowbase/Referenced.h>

#include <glow/glow_api.h>
#include <glow/AbstractState.h>
#include <glow/StateSetting.h>

namespace glow
{

class Capability;

class GLOW_API State : public AbstractState, public Referenced
{
public:
    enum Mode
    {
        DeferredMode,
        ImmediateMode
    };

public:
    State(Mode = ImmediateMode);

    static State * currentState();

    void setMode(Mode mode);
    Mode mode() const;

    void apply();

    virtual void enable(gl::GLenum capability) override;
    virtual void disable(gl::GLenum capability) override;
    virtual bool isEnabled(gl::GLenum capability) const override;
    virtual void enable(gl::GLenum capability, int index) override;
    virtual void disable(gl::GLenum capability, int index) override;
    virtual bool isEnabled(gl::GLenum capability, int index) const override;

    virtual void add(StateSetting * setting) override;

    Capability * capability(gl::GLenum capability);
    const Capability * capability(gl::GLenum capability) const;
    std::vector<Capability*> capabilities() const;

    StateSetting * setting(const StateSettingType & type);
    const StateSetting * setting(const StateSettingType & type) const;
    std::vector<StateSetting*> settings();
    std::vector<const StateSetting*> settings() const;
protected:
    virtual ~State();

    Mode m_mode;
    std::unordered_map<gl::GLenum, Capability*> m_capabilities;
    std::unordered_map<StateSettingType, StateSetting*> m_settings;

    void addCapability(Capability * capability);
    Capability* getCapability(gl::GLenum capability);
    const Capability* getCapability(gl::GLenum capability) const;
};

} // namespace glow
