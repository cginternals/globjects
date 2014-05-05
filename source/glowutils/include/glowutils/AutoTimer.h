#pragma once

#include <string>
#include <memory>

#include <glowutils/glowutils_api.h>

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
    AutoTimer(const std::string & info);
    virtual ~AutoTimer();

protected:
    static int s_numActiveInstances;

    std::string m_info;
    int m_index;

    std::unique_ptr<Timer> m_timer;
};

} // namespace glowutils
