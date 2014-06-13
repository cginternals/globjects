#pragma once

#include <vector>
#include <functional>

namespace glow {

class DebugMessage;

class DebugMessageCallback
{
public:
    using Callback = std::function<void(const DebugMessage &)>;

    DebugMessageCallback();

    void operator()(const DebugMessage & message) const;

    bool isRegistered() const;
    void setRegistered(bool registered);

    void addCallback(Callback callback);
    void clearCallbacks();
protected:
    void callCallbacks(const DebugMessage & message) const;
    void defaultAction(const DebugMessage & message) const;

    void handleError(const DebugMessage & message) const;
    void handleDebug(const DebugMessage & message) const;
protected:
    std::vector<Callback> m_callbacks;
    bool m_registered;
};

} // namespace glow
