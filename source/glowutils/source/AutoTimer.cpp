#include <glowutils/AutoTimer.h>

#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iomanip>

#include <glow/logging.h>
#include <glowutils/Timer.h>

namespace {
    // use number of digits to retrieve exp in 10^(3 exp)
    const char units[3] = { 'n', 'u', 'm' };
}

namespace glowutils
{

int AutoTimer::s_numActiveInstances = 0;

AutoTimer::AutoTimer(const std::string & info)
:   m_info(info)
,   m_index(++s_numActiveInstances)
,   m_timer(new Timer(false))
{
    m_timer->start();
}

AutoTimer::~AutoTimer()
{
    m_timer->pause();

    Timer::Duration elapsed = m_timer->elapsed();
    //double delta(static_cast<double>(m_timer->elapsed()));

    std::chrono::nanoseconds nanoDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    const unsigned int unitPrecision = std::min(3u, static_cast<unsigned int>(ceil(log10(static_cast<double>(nanoDelta.count())) / 3.0)));

    std::string unit = "s";

    if (unitPrecision < sizeof(units))
        unit.insert(0, 1, units[unitPrecision]);

    // shorten the time to nearest time unit
    double delta = static_cast<double>(nanoDelta.count()) / pow(1000.0, unitPrecision);

    glow::debug() << m_info << " took "
        << std::setprecision(4) << delta << unit
        << " (timer_" << std::setfill('0') << std::setw(2) << m_index << ").";

    --s_numActiveInstances;
}

} // namespace glowutils
