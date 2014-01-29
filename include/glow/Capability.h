#pragma once

#include <map>

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow
{

class GLOW_API Capability
{
public:
    Capability(GLenum capability);
    Capability(GLenum capability, bool enabled);

    GLenum capability() const;

    void enable();
    void disable();
    bool isEnabled() const;

    void enable(int index);
    void disable(int index);
    bool isEnabled(int index) const;

    void apply();
protected:
    GLenum m_capability;
    bool m_enabled;
    std::map<int, bool> m_indexEnabled;
};

namespace capability
{

class GLOW_API AbstractCapabilitySetting
{
public:
    enum Type {
        T_BlendFunc,
        T_LogicOp,
        T_CullFace,
        T_DepthFunc,
        T_DepthRange,
        T_SampleCoverage,
        T_PolygonMode,
        T_PolygonOffset,
        T_PointSize,
        T_PrimitiveRestartIndex,
        T_Scissor,
        T_StencilFunc,
        T_StencilOp,
        T_Custom
    };

public:
    AbstractCapabilitySetting(Type type);
    virtual ~AbstractCapabilitySetting();

    virtual void apply() = 0;

    Type type() const;
protected:
    Type m_type;
};

// concrete CapabilitySettings subclasses

class GLOW_API BlendFunc : public AbstractCapabilitySetting
{
public:
    BlendFunc(GLenum sFactor, GLenum dFactor);

    virtual void apply() override;
protected:
    GLenum sFactor;
    GLenum dFactor;
};

class GLOW_API LogicOp : public AbstractCapabilitySetting
{
public:
    LogicOp(GLenum opCode);

    virtual void apply() override;
protected:
    GLenum opCode;
};

class GLOW_API CullFace : public AbstractCapabilitySetting
{
public:
    CullFace(GLenum mode);

    virtual void apply() override;
protected:
    GLenum mode;
};

class GLOW_API DepthFunc : public AbstractCapabilitySetting
{
public:
    DepthFunc(GLenum func);

    virtual void apply() override;
protected:
    GLenum func;
};

class GLOW_API DepthRange : public AbstractCapabilitySetting
{
public:
    DepthRange(GLdouble nearVal, GLdouble farVal);
    DepthRange(GLfloat nearValF, GLfloat farValF);

    virtual void apply() override;
protected:
    GLdouble nearVal;
    GLdouble farVal;
    GLfloat nearValF;
    GLfloat farValF;
    bool useFloat;
};

class GLOW_API PointSize : public AbstractCapabilitySetting
{
public:
    PointSize(GLfloat size);

    virtual void apply() override;
protected:
    GLfloat size;
};

class GLOW_API PolygonMode : public AbstractCapabilitySetting
{
public:
    PolygonMode(GLenum face, GLenum mode);

    virtual void apply() override;
protected:
    GLenum face;
    GLenum mode;
};

class GLOW_API PolygonOffset : public AbstractCapabilitySetting
{
public:
    PolygonOffset(GLfloat factor, GLfloat units);

    virtual void apply() override;
protected:
    GLfloat factor;
    GLfloat units;
};

class GLOW_API PrimitiveRestartIndex : public AbstractCapabilitySetting
{
public:
    PrimitiveRestartIndex(GLuint index);

    virtual void apply() override;
protected:
    GLuint index;
};

class GLOW_API SampleCoverage : public AbstractCapabilitySetting
{
public:
    SampleCoverage(GLfloat value, GLboolean invert);

    virtual void apply() override;
protected:
    GLfloat value;
    GLboolean invert;
};


class GLOW_API Scissor : public AbstractCapabilitySetting
{
public:
    Scissor(GLint x, GLint y, GLsizei width, GLsizei height);

    virtual void apply() override;
protected:
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

class GLOW_API StencilFunc : public AbstractCapabilitySetting
{
public:
    StencilFunc(GLenum func, GLint ref, GLuint mask);

    virtual void apply() override;
protected:
    GLenum func;
    GLint ref;
    GLuint mask;
};

class GLOW_API StencilOp : public AbstractCapabilitySetting
{
public:
    StencilOp(GLenum fail, GLenum zFail, GLenum zPass);

    virtual void apply() override;
protected:
    GLenum fail;
    GLenum zFail;
    GLenum zPass;
};

} // namespace capability

} // namespace glow
