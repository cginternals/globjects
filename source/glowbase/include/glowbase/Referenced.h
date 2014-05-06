#pragma once

#include <glowbase/glowbase_api.h>

#include <glowbase/HeapOnly.h>

namespace glow
{

/** \brief Superclass for all classes that use reference counting in glow.
    
    The ref counter can be increased and decreased using ref() and unref().
    If the ref counter decreases to zero, the referenced objects is deleted.
    
    Referenced objects should not be copy constructed or assigned.

    \see ref_ptr
 */
class GLOWBASE_API Referenced : public HeapOnly
{
public:
    Referenced();

    void ref() const;
    void unref() const;

    int refCounter() const;

private:
    Referenced(const Referenced &) = delete;
    Referenced & operator=(const Referenced &) = delete;

protected:
    virtual ~Referenced();

private:
    mutable int m_refCounter;
};

} // namespace glowbase
