#pragma once

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

namespace globjects
{

/**
 * \brief memory provides an interface to query current memory stats of OpenGL.
 *
 * All sizes in kb.
 */
namespace memory
{
    GLOBJECTS_API gl::GLint total();
    GLOBJECTS_API gl::GLint dedicated();
    GLOBJECTS_API gl::GLint available();
    GLOBJECTS_API gl::GLint evicted();
    GLOBJECTS_API gl::GLint evictionCount();
}

} // namespace globjects
