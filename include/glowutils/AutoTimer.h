#pragma once

#include <glowutils/glowutils.h>

namespace glowutils
{

class Timer;

/** \brief Basic, scoped performance measurements with minimal interace.

    AutoTimer can be used for quick performance measurements.
    Just create an instance that, when deleted due to ending 
    scope, prints out the measured time. Example:

    \code{.cpp}
    
    {
       AutoTimer t1("Initialization"); // starts timer
       DoStuff();
    } // stops timer and prints out the elapsed time.

    \endcode

    If more control over time measurement is required, condier 
    using Timer directly.
*/
class GLOWUTILS_API AutoTimer
{
public:
    explicit AutoTimer(const char * info);
    virtual ~AutoTimer();

protected:
    static int m_numActiveInstances;

    const char * m_info;
    const int m_index;

    Timer * m_timer;
};

} // namespace glowutils
