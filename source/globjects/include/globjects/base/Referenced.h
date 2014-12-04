#pragma once

#include <memory>

#include <globjects/globjects_api.h>

#include <globjects/base/HeapOnly.h>
#include <globjects/base/Reference.h>


namespace globjects
{

/** \brief Superclass for all classes that use reference counting in globjects.
    
    The ref counter can be increased and decreased using ref() and unref().
    If the ref counter decreases to zero, the referenced objects is deleted.

    Referenced objects should not be copy constructed or assigned.

    \see ref_ptr
 */
class GLOBJECTS_API Referenced : public HeapOnly
{
public:
    Referenced();

    void ref() const;
    void unref() const;

    int refCounter() const;

    template <typename T>
    const std::shared_ptr<Reference<T>> & get_ref() const
    {
        return reinterpret_cast<const std::shared_ptr<Reference<T>> &>(m_reference);
    }
private:
    Referenced(const Referenced &) = delete;
    Referenced & operator=(const Referenced &) = delete;
protected:
    virtual ~Referenced();

private:
    mutable int m_refCounter;
    std::shared_ptr<Reference<Referenced>> m_reference;
};

} // namespace globjects
