#pragma once

// TODO: remove
#include <Windows.h>

#include <set>
#include <map>
#include <string>

#include <glow/glow.h>
#include <glow/ContextFormat.h>


namespace glow 
{

class GLOW_API Context
{
public:

	enum SwapInterval
	{
	    NoVerticalSyncronization       =  0
	,   VerticalSyncronization         =  1 ///< WGL_EXT_swap_control or GLX_EXT_swap_control 
	,   AdaptiveVerticalSyncronization = -1 ///< requires EXT_swap_control_tear
	};

    static const std::string swapIntervalString(const SwapInterval swapInterval);

public:
    Context();
    virtual ~Context();

    /** Tries to create a context with the given format on the given handle.
        If successfull, m_format is set to the format created.

        \return isValid() is returned
    */
    bool create(
        const int hWnd
    ,   const ContextFormat & format);
    void release();

    /** returns the context id. 
    */
    int id() const;

    bool makeCurrent();
    bool doneCurrent();

    void swap();

    /** The returned format refers to the created context, not the requested one.
    */
    const ContextFormat & format() const;

    /** \return true if the context was created (irrespective of format 
        verification) and if id() returns id > 0.
    */
    bool isValid() const;

    /** Swap interval relates to the context, since there might be multiple 
        shared contexts with same format, but individual swap format. 
    */
    bool setSwapInterval(const SwapInterval interval);
    bool setSwapInterval();
    SwapInterval swapInterval() const;

protected:
	SwapInterval m_swapInterval;
    ContextFormat m_format;

    HGLRC m_hRC;
    HDC   m_hDC;

    int m_hWnd;
    int m_id;
};

} // namespace glow
