#pragma once

#ifdef __APPLE__

#include "AbstractNativeContext.h"


namespace glow
{

class MacContext : public AbstractNativeContext
{
public:
    MacContext();
    virtual ~MacContext();

    virtual bool create(
        const int hWnd
    ,   ContextFormat & format);

    virtual void release();
    virtual void swap() const;

    virtual int id() const;
    virtual bool isValid() const;

    virtual bool setSwapInterval(Context::SwapInterval swapInterval) const;

    virtual bool makeCurrent() const;
    virtual bool doneCurrent() const;

protected:
//    static PIXELFORMATDESCRIPTOR toPixelFormatDescriptor(
//        const ContextFormat & format);

//    static void fromPixelFormatDescriptor(
//        ContextFormat & format
//    ,   const PIXELFORMATDESCRIPTOR & pfd);

protected:
//    HWND  m_hWnd;
//    HGLRC m_hRC;
//    HDC   m_hDC;
};

} // namespace glow

#endif
