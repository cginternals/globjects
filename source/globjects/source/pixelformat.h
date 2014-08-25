#pragma once

#include <glbinding/gl/types.h>

namespace glo {

int imageSizeInBytes(int width, int height, gl::GLenum format, gl::GLenum type);

} // namespace glo
