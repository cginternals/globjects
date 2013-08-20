#pragma once

#include <set>
#include <map>
#include <string>

#include <glow/Context.h>


namespace glow 
{

class ContextFormat;
class Version;

class AbstractNativeContext
{
public:
    AbstractNativeContext(Context & context);
    virtual ~AbstractNativeContext();

    virtual bool create(
        const int hWnd
    ,   ContextFormat & format) = 0;

    virtual void release() = 0;
    virtual void swap() const = 0;

    virtual int id() const = 0;
    virtual bool isValid() const = 0;

    virtual bool makeCurrent() const = 0;
    virtual bool doneCurrent() const = 0;

    virtual bool setSwapInterval(Context::SwapInterval swapInterval) const = 0;

    const ContextFormat & format() const;

protected:
    static bool fatalVersionDisclaimer(const Version & version);

protected:
    Context & m_context;
};

} // namespace glow
