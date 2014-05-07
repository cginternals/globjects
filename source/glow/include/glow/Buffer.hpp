#pragma once

#include <glow/Buffer.h>

namespace glow {

template <typename T>
void Buffer::setData(const std::vector<T> & data, gl::GLenum usage)
{
    setData(static_cast<gl::GLsizeiptr>(data.size() * sizeof(T)), data.data(), usage);
}

template <typename T, std::size_t Count>
void Buffer::setData(const std::array<T, Count> & data, gl::GLenum usage)
{
    setData(static_cast<gl::GLsizeiptr>(Count * sizeof(T)), data.data(), usage);
}

template <typename T>
void Buffer::setSubData(const std::vector<T> & data, gl::GLintptr offset)
{
    setSubData(offset, static_cast<gl::GLsizei>(data.size() * sizeof(T)), data.data());
}

template <typename T, std::size_t Count>
void Buffer::setSubData(const std::array<T, Count> & data, gl::GLintptr offset)
{
    setSubData(offset, static_cast<gl::GLsizei>(Count * sizeof(T)), data.data());
}

template <typename T>
void Buffer::setStorage(const std::vector<T> & data, gl::GLbitfield flags)
{
    setStorage(static_cast<gl::GLsizei>(data.size() * sizeof(T)), data.data(), flags);
}

template <typename T, std::size_t Count>
void Buffer::setStorage(const std::array<T, Count> & data, gl::GLbitfield flags)
{
    setStorage(static_cast<gl::GLsizei>(Count * sizeof(T)), data.data(), flags);
}

} // namespace glow
