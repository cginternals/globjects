#pragma once

#include <glow/glow.h>

namespace glow 
{

class ChronoTimer;

/** Timer is a wrapper of chronotimer to hide its stl usage from dll 
    export using the pimpl idiom. Its interface is modeled on a 
    stopwatch. The time updates have to be requested explicitly, thus
    simplifying usage between multiple recipients.
*/
class GLOW_API Timer
{
public:
    Timer(
        const bool start = true,
        const bool autoUpdate = true);

    virtual ~Timer();

    void setAutoUpdating(const bool autoUpdate);
    const bool autoUpdating() const;

    void update() const;
    const long double elapsed() const;

    const bool paused() const;

    void start();
    void pause();
    void stop ();
    void reset();

protected:
    ChronoTimer * m_chrono;
};

} // namespace glow