#pragma once

#include <glbinding/types.h>

#include <glow/glow_api.h>

namespace glow
{

/**
 * \brief memory provides an interface to query current memory stats of OpenGL.
 *
 * All sizes in kb.
 */
namespace memory
{
    GLOW_API gl::GLint total();
    GLOW_API gl::GLint dedicated();
    GLOW_API gl::GLint available();
    GLOW_API gl::GLint evicted();
    GLOW_API gl::GLint evictionCount();
}

} // namespace glow
