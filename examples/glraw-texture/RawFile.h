
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
    const size_t size() const;

    bool isValid() const;
    const std::string & filePath() const;

protected:
    bool readFile();
    void readRawData(std::ifstream & ifs);

protected:
    const std::string m_filePath;
    std::vector<char> m_data;

    bool m_valid;

};

