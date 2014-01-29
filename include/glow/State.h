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

class GLOW_API State : public Referenced
{
public:
    State();
    ~State();

    static State* currentState();

    void apply();

    void enable(GLenum capability);
    void disable(GLenum capability);
    bool isEnabled(GLenum capability) const;
    void enable(GLenum capability, int index);
    void disable(GLenum capability, int index);
    bool isEnabled(GLenum capability, int index) const;
    void setEnabled(GLenum capability, bool enabled);
    void setEnabled(GLenum capability, int index, bool enabled);

    // specific capability settings
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

    std::vector<Capability*> capabilities() const;

public:
    void addCapability(Capability * capability);
    void addCapabilitySetting(capability::AbstractCapabilitySetting * capabilitySetting);

protected:
    std::unordered_map<GLenum, Capability*> m_capabilities;
    std::unordered_map<unsigned, capability::AbstractCapabilitySetting*> m_capabilitySettings;

    void setToCurrent(GLenum capability);
    Capability* getCapability(GLenum capability);
};

} // namespace glow
