#pragma once

#include <GLFW/glfw3.h> ///< promote keys

#include <glowwindow/glowwindow.h>

namespace glow 
{

class GLOWWINDOW_API KeyEvent
{
public:

    enum Type
    {
        KeyPressEvent
    ,   KeyReleaseEvent
    };

public:
    KeyEvent(
        Type type
    ,   int key);
    virtual ~KeyEvent();

    int key() const;

    void setAccept(bool accept);
    bool isAccepted() const;
    bool isDiscarded() const;

    void accept();
    void discard();

protected:
    Type m_type;
    const int m_key;
    bool m_accept;
};

} // namespace glow
