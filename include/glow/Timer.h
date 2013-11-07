#pragma once

#include <glow/glow.h>

namespace glow 
{

class ChronoTimer;

/** \brief Wrapper of chronotimer to hide its stl usage.

    Timer is a wrapper of chronotimer to hide its stl usage (e.g., from dll 
    export using the pimpl idiom). Its interface is modeled on a stopwatch. The 
    time updates have to be requested explicitly, thus simplifying usage between
    multiple recipients.
*/
class GLOW_API Timer
{
public:
    Timer(
        bool start = true,
        bool autoUpdate = true);

    virtual ~Timer();

    void setAutoUpdating(const bool autoUpdate);
    bool autoUpdating() const;

    void update() const;
    long double elapsed() const;

    bool paused() const;

    void start();
    void pause();
    void stop ();
    void reset();

protected:
    ChronoTimer * m_chrono;
};

} // namespace glow
