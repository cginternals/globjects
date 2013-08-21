#pragma once

#include <cassert>
#include <algorithm>
#include <istream>
#include <fstream>

namespace glow 
{

template<typename T>
RawFile<T>::RawFile(const std::string & filePath)
:   m_filePath(filePath)
,   m_data(nullptr)
,   m_size(0)
{
    m_valid = read();
}

template<typename T>
RawFile<T>::~RawFile()
{
    delete m_data;
}

template<typename T>
bool RawFile<T>::valid() const
{
    return m_valid;
}

template<typename T>
const T * RawFile<T>::data() const
{
    return m_data;
}

template<typename T>
const size_t RawFile<T>::size() const
{
    return m_size / sizeof(T);
}

template<typename T>
bool RawFile<T>::reload(const bool failsafe)
{
    if (nullptr == m_data)
    {
        m_valid = read();
        return m_valid;
    }

    T * data = m_data;
    m_data = nullptr;

    size_t size = m_size;
    m_size = 0;

    if (read())
    {
        m_valid = true;

        delete data;
        return true;
    }

    m_data = data;
    m_size = size;

    return false;
}

template<typename T>
bool RawFile<T>::read()
{
    assert(0 == m_size);
    assert(nullptr == m_data);

    std::ifstream ifs(m_filePath, std::ios::in | std::ios::binary);

    if (!ifs)
        return false;

    m_size = ifs.rdbuf()->pubseekoff(0, ifs.end, ifs.in);
    ifs.rdbuf()->pubseekpos(0, ifs.in);

    m_data = new T[m_size / sizeof(T)];

    ifs.read((char*)m_data, m_size);
    ifs.close();

    return true;
}

} // namespace glow
