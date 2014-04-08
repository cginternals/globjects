#pragma once

#include <glow/AbstractStringSource.h>
#include <glow/ChangeListener.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

namespace glowutils 
{

class GLOWUTILS_API StringSourceDecorator : public glow::AbstractStringSource, protected glow::ChangeListener
{
public:
    StringSourceDecorator(glow::AbstractStringSource * source);
    virtual ~StringSourceDecorator();

    virtual void update();
protected:
    virtual void notifyChanged(const Changeable * changeable) override;
protected:
    glow::ref_ptr<glow::AbstractStringSource> m_internal;
};

} // namespace glowutils
