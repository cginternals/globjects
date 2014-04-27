#pragma once

#include <glow/glow_api.h>

#include <glow/HeapOnly.h>

namespace glow 
{

/** \brief Superclass for all classes that use reference counting in glow.
    
    The ref counter can be increased and decreased using ref() and unref().
    If the ref counter decreases to zero, the referenced objects is deleted.
    
    Referenced objects should not be copy constructed or assigned.

    \see ref_ptr
 */
class GLOW_API Referenced : public HeapOnly
{
public:
    Referenced();

    void ref();
    void unref();

    int refCounter() const;

protected:
    Referenced(const Referenced &);
    Referenced & operator=(const Referenced &);
    virtual ~Referenced();
private:
    int m_refCounter;
};

} // namespace glow
