#pragma once

#include <chrono>

namespace glow
{

class ChronoTimer
{
public:
    ChronoTimer(
        const bool start = true
    ,   const bool autoUpdate = true);

    virtual ~ChronoTimer();

    void setAutoUpdating(const bool autoUpdate);
    const bool autoUpdating() const;

    const bool paused() const;

    void update() const;
    const long double elapsed() const;

    void start();
    void pause();
    void stop ();
    void reset();

protected:
    bool m_paused;
    bool m_auto;

protected:
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point time_point; 

    typedef std::chrono::duration<long double, std::nano> nano;

    time_point m_t0;
    time_point m_tp; // time_point of last pausing

    mutable time_point m_t1;

    long double m_offset;
    mutable long double m_elapsed;
};

} // namespace glow
