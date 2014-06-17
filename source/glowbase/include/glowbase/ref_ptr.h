#pragma once

#include <type_traits>

namespace glow
{

class Referenced;

/** \brief The ref_ptr class provides the interface for a reference pointer.
    
    It is meant to be used together with subclasses of Referenced*
    The usage of a ref_ptr is quite the same as the work with normal pointers.
    This ref_ptr will be automatically cast to a normal pointer automatically 
    so it can be used with the glow API without any inconvenience.
    
    Example code:
    \code{.cpp}
        
        ref_ptr<VertexArrayObject> vao = new VertexArrayObject(); 
        ...
        vao->drawElements(gl::GL_TRIANGLE_STRIP, 0, 4);
    
        // vao will be deleted automatically as the ref_ptr goes out of scope

    \endcode
    
    \see Referenced
 */
template<typename T>
class ref_ptr
{
    static_assert(std::is_base_of<Referenced, T>::value, "T is not a subclass of Referenced");
public:
	ref_ptr();
	ref_ptr(T * referenced);
	ref_ptr(const ref_ptr & reference);
    ref_ptr(ref_ptr && reference);
	~ref_ptr();

    ref_ptr & operator=(const ref_ptr & reference);

    T * get() const;
    T & operator*() const;
    T * operator->() const;

    operator T *() const;

	bool operator<(const ref_ptr & reference) const;
	bool operator>(const ref_ptr & reference) const;
	bool operator==(const ref_ptr & reference) const;
    bool operator!=(const ref_ptr & reference) const;

    bool operator<(T * pointer) const;
    bool operator>(T * pointer) const;
    bool operator==(T * pointer) const;
    bool operator!=(T * pointer) const;
protected:
	void increaseRef();
	void decreaseRef();

protected:
    mutable T * m_referenced;
};

template <typename T>
ref_ptr<T> make_ref(T * object);

} // namespace glowbase

#include <glowbase/ref_ptr.hpp>
