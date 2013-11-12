#pragma once

#include <glow/ShaderSource.h>
#include <glow/ChangeListener.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

namespace glow 
{

class GLOWUTILS_API ShaderSourceDecorator : public ShaderSource, protected ChangeListener
{
public:
    ShaderSourceDecorator(ShaderSource * source);
    virtual ~ShaderSourceDecorator();

    virtual void update();
protected:
    virtual void notifyChanged();
protected:
    ref_ptr<ShaderSource> m_internal;
};

} // namespace glow
