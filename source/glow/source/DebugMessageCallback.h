#pragma once

#include <vector>
#include <functional>

#include <glow/DebugMessage.h>

namespace glow {

class DebugMessageCallback
{
public:
    using Callback = std::function<void(const DebugMessage&)>;

    DebugMessageCallback();

    void operator()(const DebugMessage & message);

    bool isRegistered() const;
    void setRegistered(bool registered);

    void addCallback(Callback callback);
    void clearCallbacks();
protected:
    void callCallbacks(const DebugMessage & message);
    void defaultAction(const DebugMessage & message);

    void handleError(const DebugMessage & message);
    void handleDebug(const DebugMessage & message);
protected:
    std::vector<Callback> m_callbacks;
    bool m_registered;
};

} // namespace glow
