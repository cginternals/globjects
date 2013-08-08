#pragma once

#include <glow/glow.h>

namespace glow 
{

class GLOW_API KeyEvent
{
public:

    enum Type
    {
        KeyPress
    ,   KeyRelease
    };

    // Key enum partially taken from Qt::Key
    enum Key
    {
        KeyUndefined    = 0x00000000

    ,   KeyEscape	    = 0x01000000
    ,   KeyTab	        = 0x01000001
    ,   KeyBacktab	    = 0x01000002
    ,   KeyBackspace	= 0x01000003
    ,   KeyReturn	    = 0x01000004
    ,   KeyEnter	    = 0x01000005
    ,   KeyInsert	    = 0x01000006
    ,   KeyDelete	    = 0x01000007
    ,   KeyPause	    = 0x01000008

    ,   KeyPrint	    = 0x01000009
    ,   KeySysReq	    = 0x0100000a
    ,   KeyClear	    = 0x0100000b
    ,   KeyHome	        = 0x01000010
    ,   KeyEnd	        = 0x01000011
    ,   KeyLeft	        = 0x01000012
    ,   KeyUp	        = 0x01000013
    ,   KeyRight	    = 0x01000014
    ,   KeyDown	        = 0x01000015
    ,   KeyPageUp	    = 0x01000016
    ,   KeyPageDown	    = 0x01000017
    ,   KeyShift	    = 0x01000020
    ,   KeyControl	    = 0x01000021  // command keys on mac
    ,   KeyMeta	        = 0x01000022  // control keys on mac, windows key on windows.
    ,   KeyAlt	        = 0x01000023
    ,   KeyAltGr	    = 0x01001103
    ,   KeyCapsLock	    = 0x01000024
    ,   KeyNumLock	    = 0x01000025
    ,   KeyScrollLock	= 0x01000026

    ,   KeyF1	        = 0x01000030
    ,   KeyF2	        = 0x01000031
    ,   KeyF3	        = 0x01000032
    ,   KeyF4	        = 0x01000033
    ,   KeyF5	        = 0x01000034
    ,   KeyF6	        = 0x01000035
    ,   KeyF7	        = 0x01000036
    ,   KeyF8	        = 0x01000037
    ,   KeyF9	        = 0x01000038
    ,   KeyF10	        = 0x01000039
    ,   KeyF11	        = 0x0100003a
    ,   KeyF12	        = 0x0100003b

    ,   KeySpace	    = 0x20
    ,   KeyExclam	    = 0x21
    ,   KeyQuoteDbl	    = 0x22
    ,   KeyNumberSign	= 0x23
    ,   KeyDollar	    = 0x24
    ,   KeyPercent	    = 0x25
    ,   KeyAmpersand	= 0x26
    ,   KeyApostrophe	= 0x27
    ,   KeyParenLeft	= 0x28
    ,   KeyParenRight	= 0x29
    ,   KeyAsterisk	    = 0x2a
    ,   KeyPlus	        = 0x2b
    ,   KeyComma	    = 0x2c
    ,   KeyMinus	    = 0x2d
    ,   KeyPeriod	    = 0x2e
    ,   KeySlash	    = 0x2f
    ,   Key0	        = 0x30
    ,   Key1	        = 0x31
    ,   Key2	        = 0x32
    ,   Key3	        = 0x33
    ,   Key4	        = 0x34
    ,   Key5	        = 0x35
    ,   Key6	        = 0x36
    ,   Key7	        = 0x37
    ,   Key8	        = 0x38
    ,   Key9	        = 0x39
    ,   KeyColon	    = 0x3a
    ,   KeySemicolon	= 0x3b
    ,   KeyLess	        = 0x3c
    ,   KeyEqual	    = 0x3d
    ,   KeyGreater	    = 0x3e
    ,   KeyQuestion	    = 0x3f
    ,   KeyAt	        = 0x40	
    ,   KeyA	        = 0x41	 
    ,   KeyB	        = 0x42	 
    ,   KeyC	        = 0x43	 
    ,   KeyD	        = 0x44	 
    ,   KeyE	        = 0x45	 
    ,   KeyF	        = 0x46	 
    ,   KeyG	        = 0x47	 
    ,   KeyH	        = 0x48	 
    ,   KeyI	        = 0x49	 
    ,   KeyJ	        = 0x4a	 
    ,   KeyK	        = 0x4b	 
    ,   KeyL	        = 0x4c	 
    ,   KeyM	        = 0x4d	 
    ,   KeyN	        = 0x4e	 
    ,   KeyO	        = 0x4f	 
    ,   KeyP	        = 0x50	 
    ,   KeyQ	        = 0x51	 
    ,   KeyR	        = 0x52	 
    ,   KeyS	        = 0x53	 
    ,   KeyT	        = 0x54	 
    ,   KeyU	        = 0x55	 
    ,   KeyV	        = 0x56	 
    ,   KeyW	        = 0x57	 
    ,   KeyX	        = 0x58	 
    ,   KeyY	        = 0x59	 
    ,   KeyZ	        = 0x5a	 
    ,   KeyBracketLeft	= 0x5b	 
    ,   KeyBackslash	= 0x5c	 
    ,   KeyBracketRight	= 0x5d	 
    ,   KeyAsciiCircum	= 0x5e	 
    ,   KeyUnderscore	= 0x5f	 
    ,   KeyQuoteLeft	= 0x60	 
    ,   KeyBraceLeft	= 0x7b	 
    ,   KeyBar	        = 0x7c	 
    ,   KeyBraceRight	= 0x7d	 
    ,   KeyAsciiTilde	= 0x7e	 
    };

public:
    KeyEvent(
        const Type type
    ,   const unsigned short key);
    virtual ~KeyEvent();

    Key key() const;

    void setAccept(const bool accept);
    bool isAccepted() const;
    bool isDiscarded() const;

    void accept();
    void discard();

protected:
    static const Key translate(const unsigned short key);

protected:
    Type m_type;
    Key m_key;

    bool m_accept;
};

} // namespace glow
