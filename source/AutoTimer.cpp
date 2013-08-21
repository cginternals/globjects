
#include <cassert>
#include <cmath>
#include <algorithm>

#include <glow/logging.h>
#include <glow/Timer.h>
#include <glow/AutoTimer.h>


namespace glow
{

int AutoTimer::m_numActiveInstances(0);

AutoTimer::AutoTimer(const char * info)
:   m_info(info)
,   m_index(++m_numActiveInstances)
,   m_timer(new Timer(false))
{
    m_timer->start();
}

AutoTimer::~AutoTimer()
{
    m_timer->pause();

    double delta(static_cast<double>(m_timer->elapsed()));

    // use number of digits to retrieve exp in 10^(3 exp)
    static const char * m_units("num ");

    const unsigned char u(std::min<unsigned char>(3, ceil(log10(delta) / 3.0)));

    // shorten the time to nearest time unit
    delta /= pow(1000.0, u);

    char dest[256];
    sprintf(dest, "%s took %.4f%cs (Timer%02i)", m_info
        , delta, m_units[u], m_index);
    debug() << const_cast<const char *>(dest);

    delete m_timer;

    assert(0 < m_numActiveInstances);
    --m_numActiveInstances;
}

} // namespace glow
