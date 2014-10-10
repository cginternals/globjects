#pragma once

#include <string>
#include <vector>

#include <glbinding/ContextHandle.h>
#include <glbinding/Version.h>


struct GLFWwindow;
struct GLFWmonitor;

class ContextFormat;


class Context
{
public:
    /** note: requires that glfw::init was previously called
        note: requires active context
    */
    static glbinding::Version retrieveVersion();

    /** note: requires that glfw::init was previously called 
    */
    static glbinding::Version maxSupportedVersion();

    enum class SwapInterval : int
    {
        NoVerticalSyncronization        =  0
    ,   VerticalSyncronization          =  1
    ,   AdaptiveVerticalSyncronization  = -1
    };

    static const std::string & swapIntervalString(SwapInterval swapInterval);

    /** strings for version, vendor, and renderer information.
        note: requires active context
    */
    static std::string version();
    static std::string vendor();
    static std::string renderer();

public:
    /** Creates a hidden window of extend 1x1 with the provided format.
    note: requires that glfw::init was previously called
    */
    static GLFWwindow * create(
        const ContextFormat & format
    ,   bool verify
    ,   int width
    ,   int height
    ,   GLFWmonitor * monitor = nullptr);

    Context(GLFWwindow * window);
    virtual ~Context();

    /** returns the context's handle - if this is somehow encapsulated tryFetchHandle for retrieval.
    */
    virtual glbinding::ContextHandle handle() const;

    /** this should be in sync to the created context, not the requested one
    */
    virtual const ContextFormat & format() const;

    /** returns true if the context was created (irrespective of format verification)
        and if handle() returns handle > 0.
    */
    virtual bool isValid() const;

    /** swap interval relates to context, since there might be multiple
        shared contexts with same format, but individual swap format.
    */
    SwapInterval swapInterval() const;

    /** note: requires active context
    */
    virtual void setSwapInterval(SwapInterval interval);

    virtual void makeCurrent() const;
    virtual void doneCurrent() const;

protected:
    /** operates by making the context current and fetching its context handle
        note: this is not thread safe, so this might result in errorneuos behavior or wrong handle.
    */
    static glbinding::ContextHandle tryFetchHandle();

protected:
    SwapInterval m_swapInterval;

    mutable ContextFormat * m_format;

    GLFWwindow * m_window;
    glbinding::ContextHandle m_handle;
};
