#pragma once

#include <functional>
#include <type_traits>

namespace glow {

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

#define collect_member(container, Class, member) collect<decltype(Class::member)>(container, [](const Class & object) { return object.member; })
#define collect_type_member(T, container, Class, member) collect<T>(container, [](const Class & object) { return object.member; })

} // namespace glowbase
