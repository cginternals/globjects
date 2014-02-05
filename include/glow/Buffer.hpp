#pragma once

#include <glow/Buffer.h>

namespace glow {

template <typename T>
void Buffer::setData(const std::vector<T> & data, GLenum usage)
{
    setData(static_cast<GLsizei>(data.size() * sizeof(T)), data.data(), usage);
}

template <typename T, std::size_t Count>
void Buffer::setData(const std::array<T, Count> & data, GLenum usage)
{
    setData(static_cast<GLsizei>(Count * sizeof(T)), data.data(), usage);
}

template <typename T>
void Buffer::setSubData(const std::vector<T> & data, GLintptr offset)
{
    setSubData(static_cast<GLsizei>(data.size() * sizeof(T)), offset, data.data());
}

template <typename T, std::size_t Count>
void Buffer::setSubData(const std::array<T, Count> & data, GLintptr offset)
{
    setSubData(static_cast<GLsizei>(Count * sizeof(T)), data.data(), offset);
}

}
