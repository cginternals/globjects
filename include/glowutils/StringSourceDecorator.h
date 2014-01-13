#pragma once

#include <glow/StringSource.h>
#include <glow/ChangeListener.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>


namespace glowutils 
{

class GLOWUTILS_API StringSourceDecorator : public glow::StringSource, protected glow::ChangeListener
{
public:
    StringSourceDecorator(glow::StringSource * source);
    virtual ~StringSourceDecorator();

    virtual void update();
protected:
    virtual void notifyChanged() override;
protected:
    glow::ref_ptr<glow::StringSource> m_internal;
};

} // namespace glowutils
