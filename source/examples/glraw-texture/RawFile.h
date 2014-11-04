
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

class RawFile
{
public:
    RawFile(const std::string & filePath);
    virtual ~RawFile();

    const char * data() const;
    size_t size() const;

    bool isValid() const;
    const std::string & filePath() const;

    // Note: this is intentionally not implemented - but fixes MSVC12 C4512 warning
    RawFile & operator=(const RawFile & rawFile);

protected:
    bool readFile();
    void readRawData(std::ifstream & ifs);

protected:
    const std::string m_filePath;
    std::vector<char> m_data;

    bool m_valid;

};

