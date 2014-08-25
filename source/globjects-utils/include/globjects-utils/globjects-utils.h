#pragma once

#include <string>

#include <globjects-utils/globjects-utils_api.h>

namespace gloutils
{

GLOBJECTS_UTILS_API void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recoursive=false);

} // namespace gloutils
