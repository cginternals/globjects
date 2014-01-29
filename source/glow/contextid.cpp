#include <GL/glew.h>

#ifdef WIN32
#include <GL/wglew.h>
#elif __APPLE__
//#include <...>
#else
#include <GL/glxew.h>
#endif

#include "contextid.h"

namespace glow {

long long getContextId()
{
    long long handle = 0;

#ifdef WIN32
    const int contextID = reinterpret_cast<int>(wglGetCurrentContext());
    handle = static_cast<long long>(contextID);
#elif __APPLE__
    //const GLXContext context = glXGetCurrentContext();
    //handle = reinterpret_cast<long long>(context);
#else
    const GLXContext context = glXGetCurrentContext();
    handle = reinterpret_cast<long long>(context);
#endif

    return handle;
}

} // namespace glow
