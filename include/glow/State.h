#pragma once

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>
#include <glow/StateSetting.h>

namespace glow
{

class Capability;

class GLOW_API StateInterface
{
public:
    virtual void enable(GLenum capability) = 0;
    virtual void disable(GLenum capability) = 0;
    virtual bool isEnabled(GLenum capability) const = 0;
    virtual void enable(GLenum capability, int index) = 0;
    virtual void disable(GLenum capability, int index) = 0;
    virtual bool isEnabled(GLenum capability, int index) const = 0;

    void setEnabled(GLenum capability, bool enabled);
    void setEnabled(GLenum capability, int index, bool enabled);

    void blendFunc(GLenum sFactor, GLenum dFactor);
    void logicOp(GLenum opcode);
    void cullFace(GLenum mode);
    void depthFunc(GLenum func);
    void depthRange(GLdouble nearVal, GLdouble farVal);
    void depthRange(GLfloat nearVal, GLfloat farVal);
    void pointSize(GLfloat size);
    void polygonMode(GLenum face, GLenum mode);
    void polygonOffset(GLfloat factor, GLfloat units);
    void primitiveRestartIndex(GLuint index);
    void sampleCoverage(GLfloat value, GLboolean invert);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void stencilFunc(GLenum func, GLint ref, GLuint mask);
    void stencilOp(GLenum fail, GLenum zFail, GLenum zPass);

    virtual void set(StateSetting * setting) = 0;
};

class GLOW_API State : public StateInterface, public Referenced
{
public:
    enum Mode
    {
        DeferredMode,
        ImmediateMode
    };

public:
    State(Mode = ImmediateMode);
    ~State();

    static State * currentState();

    void setMode(Mode mode);
    Mode mode() const;

    void apply();

    virtual void enable(GLenum capability) override;
    virtual void disable(GLenum capability) override;
    virtual bool isEnabled(GLenum capability) const override;
    virtual void enable(GLenum capability, int index) override;
    virtual void disable(GLenum capability, int index) override;
    virtual bool isEnabled(GLenum capability, int index) const override;

    virtual void set(StateSetting * setting) override;

    Capability * capability(GLenum capability);
    std::vector<Capability*> capabilities() const;

    StateSetting * setting(const StateSettingType & type);
    std::vector<StateSetting*> settings() const;
protected:
    Mode m_mode;
    std::unordered_map<GLenum, Capability*> m_capabilities;
    std::unordered_map<StateSettingType, StateSetting*> m_settings;

    void setToCurrent(GLenum capability);
    void addCapability(Capability * capability);
    Capability* getCapability(GLenum capability);
};

} // namespace glow
