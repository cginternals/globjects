

#include "ChronoTimer.h"

namespace glow
{

ChronoTimer::ChronoTimer(
    const bool start
,   const bool autoUpdate)
:   m_paused(true)
,   m_auto(autoUpdate)
,   m_t0(clock::now())
,   m_offset(0.0L)
,   m_elapsed(0.0L)
{
    m_t1 = m_t0;
    m_tp = m_t0;

    if(start)
        this->start();
}

ChronoTimer::~ChronoTimer()
{
}

void ChronoTimer::update() const
{
    m_t1 = m_paused ? m_tp : clock::now();

    const auto delta = m_t1 - m_t0;  
    m_elapsed = nano(delta).count() + m_offset;
}

const bool ChronoTimer::paused() const
{
    return m_paused;
}

void ChronoTimer::start()
{
    if(!m_paused)
        return;

    const time_point t2 = clock::now();
    m_offset -= nano(t2 - m_tp).count();

    m_t1 = t2;
    m_paused = false;
}

void ChronoTimer::pause()
{
    if(m_paused)
        return;

    m_tp = clock::now();
    m_paused = true;
}

void ChronoTimer::stop()
{
    pause();
    reset();
}

void ChronoTimer::reset()
{
    m_offset = 0.0L;
    m_elapsed = 0.0L;

    m_t0 = clock::now();
    m_t1 = m_t0;
    m_tp = m_t0;
}

const long double ChronoTimer::elapsed() const
{
    if(m_auto)
        update();

    return m_elapsed;
}

void ChronoTimer::setAutoUpdating(const bool auto_update)
{
    m_auto = auto_update;
}

const bool ChronoTimer::autoUpdating() const
{
    return m_auto;
}

} // namespace glow
