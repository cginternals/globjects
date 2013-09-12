
#include <cassert>

#include <unordered_map>

#ifdef WIN32
#include <windows.h>
#elif __APPLE__

#else

#endif

#include <glow/logging.h>
#include <glowwindow/KeyEvent.h>


namespace glow
{

KeyEvent::KeyEvent(
    const Type type
,   const unsigned short key)
:   m_type(type)
, m_key(translate(key))
,   m_accept(false)
{
}

KeyEvent::~KeyEvent()
{
}

KeyEvent::Key KeyEvent::key() const
{
    return m_key;
}

void KeyEvent::setAccept(const bool accept)
{
    m_accept = accept;
}

bool KeyEvent::isAccepted() const
{
    return m_accept;
}

bool KeyEvent::isDiscarded() const
{
    return !m_accept;
}

void KeyEvent::accept()
{
    m_accept = true;
}
    
void KeyEvent::discard()
{
    m_accept = false;
}

const KeyEvent::Key KeyEvent::translate(const unsigned short key)
{
    static const std::unordered_map<unsigned short, KeyEvent::Key> keys =
    {
#ifdef WIN32

        { VK_BACK   , KeyBackspace }
    ,   { VK_TAB    , KeyTab       }
    ,   { VK_CLEAR  , KeyClear     }
    ,   { VK_RETURN , KeyReturn    }
    ,   { VK_SHIFT  , KeyShift     }
    ,   { VK_CONTROL, KeyControl   }
    ,   { VK_MENU   , KeyAlt       }
    ,   { VK_PAUSE  , KeyPause     }
    ,   { VK_CAPITAL, KeyCapsLock  }
    ,   { VK_ESCAPE , KeyEscape    }
    ,   { VK_SPACE  , KeySpace     }
    ,   { VK_PRIOR  , KeyPageUp    }
    ,   { VK_NEXT   , KeyPageDown  }
    ,   { VK_END    , KeyEnd       }
    ,   { VK_HOME   , KeyHome      }
    ,   { VK_LEFT   , KeyLeft      }
    ,   { VK_UP     , KeyUp        }
    ,   { VK_RIGHT  , KeyRight     }
    ,   { VK_DOWN   , KeyDown      }
    ,   { VK_PRINT  , KeyPrint     }
    ,   { VK_INSERT , KeyInsert    }
    ,   { VK_DELETE , KeyDelete    }

    ,   { 0x30, Key0 }
    ,   { 0x31, Key1 }
    ,   { 0x32, Key2 }
    ,   { 0x33, Key3 }
    ,   { 0x34, Key4 }
    ,   { 0x35, Key5 }
    ,   { 0x36, Key6 }
    ,   { 0x37, Key7 }
    ,   { 0x38, Key8 }
    ,   { 0x39, Key9 }
    
    ,   { 0x41, KeyA }
    ,   { 0x42, KeyB }
    ,   { 0x43, KeyC }
    ,   { 0x44, KeyD }
    ,   { 0x45, KeyE }
    ,   { 0x46, KeyF }
    ,   { 0x47, KeyG }
    ,   { 0x48, KeyH }
    ,   { 0x49, KeyI }
    ,   { 0x4A, KeyJ }
    ,   { 0x4B, KeyK }
    ,   { 0x4C, KeyL }
    ,   { 0x4D, KeyM }
    ,   { 0x4E, KeyN }
    ,   { 0x4F, KeyO }
    ,   { 0x50, KeyP }
    ,   { 0x51, KeyQ }
    ,   { 0x52, KeyR }
    ,   { 0x53, KeyS }
    ,   { 0x54, KeyT }
    ,   { 0x55, KeyU }
    ,   { 0x56, KeyV }
    ,   { 0x57, KeyW }
    ,   { 0x58, KeyX }
    ,   { 0x59, KeyY }
    ,   { 0x5A, KeyZ }

    ,   { VK_NUMPAD0, Key0 }
    ,   { VK_NUMPAD1, Key1 }
    ,   { VK_NUMPAD2, Key2 }
    ,   { VK_NUMPAD3, Key3 }
    ,   { VK_NUMPAD4, Key4 }
    ,   { VK_NUMPAD5, Key5 }
    ,   { VK_NUMPAD6, Key6 }
    ,   { VK_NUMPAD7, Key7 }
    ,   { VK_NUMPAD8, Key8 }
    ,   { VK_NUMPAD9, Key9 }

    //,   { VK_MULTIPLY , KeyMultiply  }
    //,   { VK_ADD      , KeyAdd       }
    //,   { VK_SEPARATOR, KeySeparator }
    //,   { VK_SUBTRACT , KeySubtract  }
    //,   { VK_DECIMAL  , KeyDecimal   }
    //,   { VK_DIVIDE   , KeyDivide    }

    ,   { VK_F1, KeyF1 }
    ,   { VK_F2, KeyF2 }
    ,   { VK_F3, KeyF3 }
    ,   { VK_F4, KeyF4 }
    ,   { VK_F5, KeyF5 }
    ,   { VK_F6, KeyF6 }
    ,   { VK_F7, KeyF7 }
    ,   { VK_F8, KeyF8 }
    ,   { VK_F9, KeyF9 }
    ,   { VK_F10, KeyF10 }
    ,   { VK_F11, KeyF11 }
    ,   { VK_F12, KeyF12 }

    ,   { VK_LSHIFT  , KeyShift   }
    ,   { VK_RSHIFT  , KeyShift   }
    ,   { VK_LCONTROL, KeyControl }
    ,   { VK_RCONTROL, KeyControl }

    ,   { VK_OEM_PLUS  , KeyPlus   }
    ,   { VK_OEM_COMMA , KeyComma  }
    ,   { VK_OEM_MINUS , KeyMinus  }
    ,   { VK_OEM_PERIOD, KeyPeriod }

    //,   { VK_OEM_1, }   // For any country/region, the '.' key
    //,   { VK_OEM_2, }   // For the US standard keyboard, the '/?' key
    //,   { VK_OEM_3, }   // For the US standard keyboard, the '`~' key
    //,   { VK_OEM_4, }   // For the US standard keyboard, the '[{' key
    //,   { VK_OEM_5, }   // For the US standard keyboard, the '\|' key
    //,   { VK_OEM_6, }   // For the US standard keyboard, the ']}' key
    //,   { VK_OEM_7, }   // For the US standard keyboard, the 'single-quote/double-quote' key
    //,   { VK_OEM_8, } 

    //,   { VK_OEM_102, } //  angle bracket or the backslash 

#elif __APPLE__

#else

#endif

    };

    auto f = keys.find(key);
    if (f == keys.cend())
        return KeyUndefined;

    return f->second;
}

} // namespace glow
