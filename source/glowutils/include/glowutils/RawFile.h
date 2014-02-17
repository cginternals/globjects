#pragma once

#include <string>
#include <vector>

#include <glowutils/glowutils.h>

namespace glowutils
{

/** \brief Fast binary file to memory dump.

    The RawFile allows fast reading of binary/raw data and is intended
    to be used for assets, e.g., created with glraw. This is to reduce the
    3rd party overhead when reading assets (3d models, 1d/2d/3d images, etc.,).

    \code{.cpp}

        RawFile<char> raw(m_filePath);
        if (raw.valid())
            doStuffWith(raw.data(), raw.size());

    \endcode

    \see ShaderFile
*/
template<typename T>
class RawFile
{
public:
    RawFile(const std::string & filePath);
    virtual ~RawFile();

    const T * data() const;
    size_t size() const;

    bool valid() const;
    const std::string & filePath() const;

	bool read();

protected:
	const std::string m_filePath;
    std::vector<T> m_data;

    bool m_valid;
};

} // namespace glowutils

#include <glowutils/RawFile.hpp>
