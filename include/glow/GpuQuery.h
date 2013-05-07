
#pragma once

#include <GL/glew.h>

#include <string>

#include <glow/declspec.h>

namespace glow
{

class GLOW_API GPUQuery
{
public:
    static const GLint   queryi(const GLenum penum);
    static const std::string querys(const GLenum penum);
    static const std::string glew  (const GLenum penum);

    static const std::string vendor();
    static const std::string renderer();
    static const std::string version();
    static const std::string glewVersion();

    static const bool isCoreProfile();

    // Checks for an opengl error and prints to console if one occurred
    static const bool error(
        const char * file
    ,   const int line);

    static const std::string glewError(const GLenum penum);

    // Extension Info
    
    static const bool extensionSupported(const char * extension);
    static const bool extensionSupported(const std::string & extension);

    // Memory Info (return -1 if query is not possible or failed)

    static const GLint totalMemory();
    static const GLint dedicatedMemory();
    static const GLint availableMemory();
    static const GLint evictionCount();
    static const GLint evictedMemory();

protected:
    static const GLint memoryInfo(const GLenum penum);
};


#ifdef _DEBUG
    #define glError() GPUQuery::error(__FILE__, __LINE__)
#else
    #define glError() false
#endif

} // namespace glow