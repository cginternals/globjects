#pragma once

#include <glbinding/gl/types.h>

namespace globjects {

int imageSizeInBytes(int width, int height, gl::GLenum format, gl::GLenum type);

} // namespace globjects
