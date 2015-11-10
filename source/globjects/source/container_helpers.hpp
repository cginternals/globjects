#pragma once

#include <functional>
#include <type_traits>

template <typename T, class InputIterator, class Function>
std::vector<T> collect(InputIterator first, InputIterator last, Function mapper)
{
    std::vector<T> result;

    while (first != last)
    {
        result.push_back(mapper(*first));
        ++first;
    }
    return result;
}

template <typename T, class Container, class Function>
std::vector<T> collect(Container container, Function mapper)
{
    return collect<T, typename Container::const_iterator, Function>(container.begin(), container.end(), mapper);
}

template<class Container, class Class, class MemberType>
std::vector<MemberType> collect_member(Container container, MemberType Class::*memberPointer)
{
    return collect<MemberType>(container, [memberPointer](const Class & object) { return (object.*memberPointer); });
}

template<class ReturnType, class Container, class Class, class MemberType>
std::vector<ReturnType> collect_type_member(Container container, MemberType Class::*memberPointer)
{
    return collect<ReturnType>(container, [memberPointer](const Class & object) { return static_cast<ReturnType>(object.*memberPointer); });
}
