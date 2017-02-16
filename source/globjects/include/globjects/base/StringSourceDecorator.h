
#pragma once

#include <globjects/base/ChangeListener.h>
#include <globjects/base/AbstractStringSource.h>

#include <globjects/globjects_api.h>


namespace globjects
{


class GLOBJECTS_API StringSourceDecorator : public globjects::AbstractStringSource, protected globjects::ChangeListener
{
public:
    StringSourceDecorator(globjects::AbstractStringSource * source);

    virtual void update();
protected:
    virtual ~StringSourceDecorator();

    virtual void notifyChanged(const Changeable * changeable) override;

protected:
    globjects::AbstractStringSource * m_internal;
};


} // namespace globjects
