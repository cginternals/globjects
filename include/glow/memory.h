#pragma once

#include <GL/glew.h>

#include <glow/glow.h>

namespace glow
{

/**
 * \brief memory provides an interface to query current memory stats of OpenGL.
 */
namespace memory
{
    /** all sizes in kb */
    GLOW_API GLint total();
    GLOW_API GLint dedicated();
    GLOW_API GLint available();
    GLOW_API GLint evicted();
    GLOW_API GLint evictionCount();

    GLOW_API GLint get(GLenum pname);
}

} // namespace glow
