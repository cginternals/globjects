#pragma once

#include <GL/glew.h>

namespace glow {

int imageSizeInBytes(int width, int height, GLenum format, GLenum type);

} // namespace glow
