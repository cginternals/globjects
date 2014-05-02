#pragma once

#include <glowutils/RawFile.h>

#include <algorithm>
#include <fstream>

#include <glow/logging.h>

namespace glowutils 
{

template<typename T>
RawFile<T>::RawFile(const std::string & filePath)
:   m_filePath(filePath)
,   m_valid(false)
{
    read();
}

template<typename T>
RawFile<T>::~RawFile()
{
}

template<typename T>
bool RawFile<T>::valid() const
{
    return m_valid;
}

template<typename T>
const T * RawFile<T>::data() const
{
    return &m_data.data()[0];
}

template<typename T>
size_t RawFile<T>::size() const
{
    return m_data.size();
}

template<typename T>
bool RawFile<T>::read()
{
    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!ifs)
    {
        glow::warning() << "Reading from file \"" << m_filePath << "\" failed.";
        return false;
    }

    const std::streamsize size = static_cast<const std::streamsize>(ifs.tellg());

    ifs.seekg(0, std::ios::beg);

    m_data.resize(size / sizeof(T));

    ifs.read(reinterpret_cast<char*>(&m_data[0]), size);
    ifs.close();

    m_valid = true;
    return true;
}

} // namespace glowutils
