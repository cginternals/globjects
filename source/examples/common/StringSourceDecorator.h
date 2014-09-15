#pragma once

#include <globjects/base/ChangeListener.h>
#include <globjects/base/ref_ptr.h>
#include <globjects/base/AbstractStringSource.h>

class StringSourceDecorator : public globjects::AbstractStringSource, protected globjects::ChangeListener
{
public:
    StringSourceDecorator(globjects::AbstractStringSource * source);

    virtual void update();
protected:
    virtual ~StringSourceDecorator();

    virtual void notifyChanged(const Changeable * changeable) override;
protected:
    globjects::ref_ptr<globjects::AbstractStringSource> m_internal;
};
