#pragma once

#include <string>

namespace glow {
namespace internal {

class FileReader
{
public:
	static bool readFile(const std::string& filePath, std::string& content);
};

} // namespace internal
} // namespace glow
