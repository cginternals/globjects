
#pragma once


#include <cassert>


namespace globjects 
{


template<class T>
T * Singleton<T>::s_instance(nullptr);

template<class T>
T * Singleton<T>::instance()
{
    if (!s_instance)
    {
        s_instance = new T();
    }

    return s_instance;
}

template<class T>
Singleton<T>::Singleton()
{
    assert(!s_instance);
}

template<class T>
Singleton<T>::~Singleton()
{
    assert(s_instance);

    delete s_instance;
}


} // namespace globjects
