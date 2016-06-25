
#pragma once

#include <functional>
#include <type_traits>
#include <vector>


template <typename T, class InputIterator, class Function>
std::vector<T> collect(InputIterator first, InputIterator last, Function mapper);

template <typename T, class Container, class Function>
std::vector<T> collect(Container container, Function mapper);

template<class Container, class Class, class MemberType>
std::vector<MemberType> collect_member(Container container, MemberType Class::*memberPointer);

template<class ReturnType, class Container, class Class, class MemberType>
std::vector<ReturnType> collect_type_member(Container container, MemberType Class::*memberPointer);


#include "container_helpers.inl"
