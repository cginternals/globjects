#pragma once

#include <string>

#include <glow/glow.h>

namespace glow
{

template<typename T>
class GLOW_API RawFile
{
public:
    RawFile(const std::string & filePath);
    virtual ~RawFile();

    const T * data() const;
    const size_t size() const;

    bool valid() const;
    const std::string & filePath() const;

    bool reload(const bool failsafe = true);

protected:
	bool read();

protected:
	const std::string m_filePath;

    T * m_data;
    size_t m_size;

    bool m_valid;
};

} // namespace glow

#include <glow/RawFile.hpp>
