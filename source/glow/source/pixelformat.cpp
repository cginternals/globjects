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
        case gl::UNSIGNED_BYTE:
        case gl::BYTE:
            return 1;

        case gl::UNSIGNED_SHORT:
        case gl::SHORT:
            return 2;

        case gl::UNSIGNED_INT:
        case gl::INT:
            return 4;

        case gl::HALF_FLOAT:
            return 2;

        case gl::FLOAT:
            return 4;

        default:
            return 1;
    }
}

int numberOfComponents(gl::GLenum format)
{
    switch (format)
    {
        case gl::RED:
        case gl::GREEN:
        case gl::BLUE:
        case gl::RED_INTEGER:
        case gl::GREEN_INTEGER:
        case gl::BLUE_INTEGER:

        case gl::ALPHA:
        case gl::LUMINANCE:
        case gl::LUMINANCE_ALPHA:

        case gl::STENCIL_INDEX:
        case gl::COLOR_INDEX:
        case gl::DEPTH_COMPONENT:
        case gl::DEPTH_STENCIL:
            return 1;

        case gl::RG:
        case gl::RG_INTEGER:
            return 2;

        case gl::RGB:
        case gl::BGR:
        case gl::RGB_INTEGER:
        case gl::BGR_INTEGER:
            return  3;

        case gl::RGBA:
        case gl::BGRA:
        case gl::RGBA_INTEGER:
        case gl::BGRA_INTEGER:
            return 4;
        default:
            return 1;
    }
}

int bytesPerPixel(gl::GLenum format, gl::GLenum type)
{
    switch (type) // several components encoded in type, disregard component count
    {
        case gl::UNSIGNED_BYTE_3_3_2:
        case gl::UNSIGNED_BYTE_2_3_3_REV:
            return 1; // 8 bit

        case gl::UNSIGNED_SHORT_5_6_5:
        case gl::UNSIGNED_SHORT_5_6_5_REV:
        case gl::UNSIGNED_SHORT_4_4_4_4:
        case gl::UNSIGNED_SHORT_4_4_4_4_REV:
        case gl::UNSIGNED_SHORT_5_5_5_1:
        case gl::UNSIGNED_SHORT_1_5_5_5_REV:
            return 2; // 16 bit

        case gl::UNSIGNED_INT_8_8_8_8:
        case gl::UNSIGNED_INT_8_8_8_8_REV:
        case gl::UNSIGNED_INT_10_10_10_2:
        case gl::UNSIGNED_INT_2_10_10_10_REV:
        case gl::UNSIGNED_INT_24_8:
        case gl::UNSIGNED_INT_10F_11F_11F_REV:
        case gl::UNSIGNED_INT_5_9_9_9_REV:
        case gl::FLOAT_32_UNSIGNED_INT_24_8_REV:
            return 4; // 32 bit
    }

    return numberOfComponents(format) * byteSize(type);
}

}

namespace glow {

int imageSizeInBytes(int width, int height, gl::GLenum format, gl::GLenum type)
{
    if (type == gl::BITMAP)
    {
        // handle differently?
        warning() << "imageSizeInBytes: gl::BITMAP not implemented yet";
        return -1;
    }

    int pixelSize = bytesPerPixel(format, type);

    int rowSize = pixelSize * width;

    int alignment = getInteger(gl::PACK_ALIGNMENT); // can be 1, 2, 4 or 8

    rowSize = nextMultiple(rowSize, alignment);

    return rowSize * height;
}

} // namespace glow
