#pragma once

#include <glow/StringSource.h>
#include <glow/ChangeListener.h>
#include <glow/ref_ptr.h>

#include <glowutils/glowutils.h>

namespace glow 
{

class GLOWUTILS_API StringSourceDecorator : public StringSource, protected ChangeListener
{
public:
    StringSourceDecorator(StringSource * source);
    virtual ~StringSourceDecorator();

    virtual void update();
protected:
    virtual void notifyChanged();
protected:
    ref_ptr<StringSource> m_internal;
};

} // namespace glow
