#pragma once

#include <vector>
#include <functional>

#include <glow/DebugMessage.h>

class DebugMessageCallback
{
public:
    using Callback = std::function<void(const glow::DebugMessage&)>;

    DebugMessageCallback();

    void operator()(const glow::DebugMessage & message);

    bool isRegistered() const;
    void setRegistered(bool registered);

    void addCallback(Callback callback);
    void clearCallbacks();
protected:
    void callCallbacks(const glow::DebugMessage & message);
    void defaultAction(const glow::DebugMessage & message);

    void handleError(const glow::DebugMessage & message);
    void handleDebug(const glow::DebugMessage & message);
protected:
    std::vector<Callback> m_callbacks;
    bool m_registered;
};
