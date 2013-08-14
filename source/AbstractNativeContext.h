#pragma once

#include <set>
#include <map>
#include <string>

#include <glow/ContextFormat.h>
#include <glow/Context.h>


namespace glow 
{

class AbstractNativeContext
{
public:
    AbstractNativeContext();
    virtual ~AbstractNativeContext();

    virtual bool create(
        const int hWnd
    ,   ContextFormat & format) = 0;

    virtual void release() = 0;
    virtual bool swap() const = 0;

    virtual int id() const = 0;
    virtual bool isValid() const = 0;

    virtual bool makeCurrent() const = 0;
    virtual bool doneCurrent() const = 0;

    virtual bool setSwapInterval(Context::SwapInterval swapInterval) const = 0;
};

} // namespace glow
