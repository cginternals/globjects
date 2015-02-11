#pragma once

#include <type_traits>
#include <memory>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/Reference.h>

namespace globjects
{

class Referenced;

/** \brief The weak_ptr class provides the interface for a weak pointer.
    
    It is meant to be used together with subclasses of Referenced*
    The usage of a weak_ptr is quite the same as the work with the std::weak_ptr.
    
    Example code:
    \code{.cpp}
        
        weak_ptr<FrameBuffer> weak_fbo = Registry::current().objects().defaultFBO();
        
        ref_ptr<FrameBuffer> fbo = weak_fbo->lock();
        
        fbo->setDrawBuffers( ... );
    \endcode
    
    \see Referenced, ref_ptr
 */
template<typename T>
class weak_ptr
{
//    static_assert(std::is_base_of<Referenced, T>::value, "T is not a subclass of Referenced");

public:
	weak_ptr();
	weak_ptr(T * referenced);
	weak_ptr(const weak_ptr & reference);
	weak_ptr(weak_ptr && reference);
	weak_ptr(const ref_ptr<T> & reference);
	~weak_ptr();

    weak_ptr & operator=(T * referenced);
    weak_ptr & operator=(const weak_ptr & reference);
    weak_ptr & operator=(weak_ptr && reference);
    weak_ptr & operator=(const ref_ptr<T> & referenced);

    ref_ptr<T> lock() const;

protected:
    std::shared_ptr<Reference<T>> m_reference;
};

} // namespace globjects

#include <globjects/base/weak_ptr.hpp>
