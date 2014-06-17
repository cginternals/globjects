#include "pixelformat.h"

#include <glbinding/constants.h>

#include <glow/glow.h>
#include <glow/logging.h>

namespace {

int nextMultiple(int n, int k)
{
    return n + (k-n%k)%k;
}

int byteSize(gl::GLenum type)
{
    switch (type)
    {
        case gl::GL_UNSIGNED_BYTE:
        case gl::GL_BYTE:
            return 1;

        case gl::GL_UNSIGNED_SHORT:
        case gl::GL_SHORT:
            return 2;

        case gl::GL_UNSIGNED_INT:
        case gl::GL_INT:
            return 4;

        case gl::GL_HALF_FLOAT:
            return 2;

        case gl::GL_FLOAT:
            return 4;

        default:
            return 1;
    }
}

int numberOfComponents(gl::GLenum format)
{
    switch (format)
    {
        case gl::GL_RED:
        case gl::GL_GREEN:
        case gl::GL_BLUE:
        case gl::GL_RED_INTEGER:
        case gl::GL_GREEN_INTEGER:
        case gl::GL_BLUE_INTEGER:

        case gl::GL_ALPHA:
        case gl::GL_LUMINANCE:
        case gl::GL_LUMINANCE_ALPHA:

        case gl::GL_STENCIL_INDEX:
        case gl::GL_COLOR_INDEX:
        case gl::GL_DEPTH_COMPONENT:
        case gl::GL_DEPTH_STENCIL:
            return 1;

        case gl::GL_RG:
        case gl::GL_RG_INTEGER:
            return 2;

        case gl::GL_RGB:
        case gl::GL_BGR:
        case gl::GL_RGB_INTEGER:
        case gl::GL_BGR_INTEGER:
            return  3;

        case gl::GL_RGBA:
        case gl::GL_BGRA:
        case gl::GL_RGBA_INTEGER:
        case gl::GL_BGRA_INTEGER:
            return 4;
        default:
            return 1;
    }
}

int bytesPerPixel(gl::GLenum format, gl::GLenum type)
{
    switch (type) // several components encoded in type, disregard component count
    {
        case gl::GL_UNSIGNED_BYTE_3_3_2:
        case gl::GL_UNSIGNED_BYTE_2_3_3_REV:
            return 1; // 8 bit

        case gl::GL_UNSIGNED_SHORT_5_6_5:
        case gl::GL_UNSIGNED_SHORT_5_6_5_REV:
        case gl::GL_UNSIGNED_SHORT_4_4_4_4:
        case gl::GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case gl::GL_UNSIGNED_SHORT_5_5_5_1:
        case gl::GL_UNSIGNED_SHORT_1_5_5_5_REV:
            return 2; // 16 bit

        case gl::GL_UNSIGNED_INT_8_8_8_8:
        case gl::GL_UNSIGNED_INT_8_8_8_8_REV:
        case gl::GL_UNSIGNED_INT_10_10_10_2:
        case gl::GL_UNSIGNED_INT_2_10_10_10_REV:
        case gl::GL_UNSIGNED_INT_24_8:
        case gl::GL_UNSIGNED_INT_10F_11F_11F_REV:
        case gl::GL_UNSIGNED_INT_5_9_9_9_REV:
        case gl::GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
            return 4; // 32 bit

        default:
            break;
    }

    return numberOfComponents(format) * byteSize(type);
}

}

namespace glow {

int imageSizeInBytes(int width, int height, gl::GLenum format, gl::GLenum type)
{
    if (type == gl::GL_BITMAP)
    {
        // handle differently?
        warning() << "imageSizeInBytes: gl::GL_BITMAP not implemented yet";
        return -1;
    }

    int pixelSize = bytesPerPixel(format, type);

    int rowSize = pixelSize * width;

    int alignment = getInteger(gl::GL_PACK_ALIGNMENT); // can be 1, 2, 4 or 8

    rowSize = nextMultiple(rowSize, alignment);

    return rowSize * height;
}

} // namespace glow
