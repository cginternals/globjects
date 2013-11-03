#pragma once

#if defined (__linux__) || defined (__APPLE__)

#include <GL/glx.h>
#include <X11/Xlib.h>

#include "AbstractNativeContext.h"


namespace glow
{

class GLxContext : public AbstractNativeContext
{
public:
    GLxContext(Context & context);
    virtual ~GLxContext();

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

public:
//    static PIXELFORMATDESCRIPTOR toPixelFormatDescriptor(
//        const ContextFormat & format);

//    static void fromPixelFormatDescriptor(
//        ContextFormat & format
//    ,   const PIXELFORMATDESCRIPTOR & pfd);

    static Display * getOrOpenDisplay();
    static void closeDisplay();

protected:
    static Display * s_display;

protected:
    Display * m_display;
    ::Window m_hWnd;

    ::GLXContext m_context;
    int m_id;

//    HGLRC m_hRC;
//    HDC   m_hDC;
};

} // namespace glow

#endif
