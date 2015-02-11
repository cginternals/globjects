#pragma once

#include <utility>

#include <globjects/base/weak_ptr.h>

#include <globjects/base/Referenced.h>


namespace globjects
{

template<typename T>
weak_ptr<T>::weak_ptr()
{
}

template<typename T>
weak_ptr<T>::weak_ptr(T * referenced)
: m_reference(referenced ? referenced->template get_ref<T>() : nullptr)
{
}

template<typename T>
weak_ptr<T>::weak_ptr(const weak_ptr & reference)
: m_reference(reference.m_reference)
{
}

template<typename T>
weak_ptr<T>::weak_ptr(weak_ptr && reference)
: m_reference(std::move(reference.m_reference))
{
}

template<typename T>
weak_ptr<T>::weak_ptr(const ref_ptr<T> & referenced)
: m_reference(referenced->template get_ref<T>())
{
}

template<typename T>
weak_ptr<T>::~weak_ptr()
{
}

template<typename T>
weak_ptr<T> & weak_ptr<T>::operator=(T * referenced)
{
    m_reference = referenced ? referenced->template get_ref<T>() : nullptr;
    
    return *this;
}

template<typename T>
weak_ptr<T> & weak_ptr<T>::operator=(const weak_ptr & reference)
{
    m_reference = reference.m_reference;
    
    return *this;
}

template<typename T>
weak_ptr<T> & weak_ptr<T>::operator=(weak_ptr && reference)
{
    m_reference = std::move(reference.m_reference);
    
    return *this;
}

template<typename T>
weak_ptr<T> & weak_ptr<T>::operator=(const ref_ptr<T> & referenced)
{
    m_reference = referenced->template get_ref<T>();
    
    return *this;
}

template<typename T>
ref_ptr<T> weak_ptr<T>::lock() const
{
    return ref_ptr<T>(*m_reference.get());
}

} // namespace globjects
