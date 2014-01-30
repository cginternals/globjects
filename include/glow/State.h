#pragma once

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

namespace glow
{

class Capability;

namespace capability {

class AbstractCapabilitySetting;

} // namespace capability

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

    virtual void blendFunc(GLenum sFactor, GLenum dFactor) = 0;
    virtual void logicOp(GLenum opcode) = 0;
    virtual void cullFace(GLenum mode) = 0;
    virtual void depthFunc(GLenum func) = 0;
    virtual void depthRange(GLdouble nearVal, GLdouble farVal) = 0;
    virtual void depthRange(GLfloat nearVal, GLfloat farVal) = 0;
    virtual void pointSize(GLfloat size) = 0;
    virtual void polygonMode(GLenum face, GLenum mode) = 0;
    virtual void polygonOffset(GLfloat factor, GLfloat units) = 0;
    virtual void primitiveRestartIndex(GLuint index) = 0;
    virtual void sampleCoverage(GLfloat value, GLboolean invert) = 0;
    virtual void scissor(GLint x, GLint y, GLsizei width, GLsizei height) = 0;
    virtual void stencilFunc(GLenum func, GLint ref, GLuint mask) = 0;
    virtual void stencilOp(GLenum fail, GLenum zFail, GLenum zPass) = 0;
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

    virtual void blendFunc(GLenum sFactor, GLenum dFactor) override;
    virtual void logicOp(GLenum opcode) override;
    virtual void cullFace(GLenum mode) override;
    virtual void depthFunc(GLenum func) override;
    virtual void depthRange(GLdouble nearVal, GLdouble farVal) override;
    virtual void depthRange(GLfloat nearVal, GLfloat farVal) override;
    virtual void pointSize(GLfloat size) override;
    virtual void polygonMode(GLenum face, GLenum mode) override;
    virtual void polygonOffset(GLfloat factor, GLfloat units) override;
    virtual void primitiveRestartIndex(GLuint index) override;
    virtual void sampleCoverage(GLfloat value, GLboolean invert) override;
    virtual void scissor(GLint x, GLint y, GLsizei width, GLsizei height) override;
    virtual void stencilFunc(GLenum func, GLint ref, GLuint mask) override;
    virtual void stencilOp(GLenum fail, GLenum zFail, GLenum zPass) override;

    bool hasCapability(GLenum capability);
    Capability* capability(GLenum capability);

    std::vector<Capability*> capabilities() const;

public:
    void addCapability(Capability * capability);
    void addCapabilitySetting(capability::AbstractCapabilitySetting * capabilitySetting);

protected:
    Mode m_mode;
    std::unordered_map<GLenum, Capability*> m_capabilities;
    std::unordered_map<unsigned, capability::AbstractCapabilitySetting*> m_capabilitySettings;

    void setToCurrent(GLenum capability);
    Capability* getCapability(GLenum capability);
};

} // namespace glow
