#pragma once

namespace glo
{

template<class T>
class Singleton
{
public:
    static T * instance();

protected:
    explicit Singleton();
    virtual ~Singleton();

protected:
    static T * s_instance;
};

} // namespace glo

#include <globjects-base/Singleton.hpp>
