
#pragma once

#include <string>

namespace glow 
{

class FileReader
{
public:
	static bool readFile(
        const std::string & filePath
    ,   std::string & data);
};

} // namespace glow

