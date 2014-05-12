
#include "contextid.h"

#ifdef WIN32
#include <windows.h>
#elif __APPLE__
//#include <...>
#else
#include <GL/glxew.h>
#endif


namespace glow {

ContextID getContextId()
{
    ContextID id = 0;

#ifdef WIN32
    const HGLRC context = wglGetCurrentContext();
    id = reinterpret_cast<ContextID>(context);
#elif __APPLE__
    //const GLXContext context = glXGetCurrentContext();
    //handle = reinterpret_cast<long long>(context);
#else
    const GLXContext context = glXGetCurrentContext();
    id = reinterpret_cast<ContextID>(context);
#endif

    return id;
}

} // namespace glow
