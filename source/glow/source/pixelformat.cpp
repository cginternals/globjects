#include "pixelformat.h"

#include <glow/glow.h>
#include <glow/logging.h>

namespace {

int nextMultiple(int n, int k)
{
    return n + (k-n%k)%k;
}

int byteSize(GLenum type)
{
    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_BYTE:
            return 1;

        case GL_UNSIGNED_SHORT:
        case GL_SHORT:
            return 2;

        case GL_UNSIGNED_INT:
        case GL_INT:
            return 4;

        case GL_HALF_FLOAT:
            return 2;

        case GL_FLOAT:
            return 4;

        default:
            return 1;
    }
}

int numberOfComponents(GLenum format)
{
    switch (format)
    {
        case GL_RED:
        case GL_GREEN:
        case GL_BLUE:
        case GL_RED_INTEGER:
        case GL_GREEN_INTEGER:
        case GL_BLUE_INTEGER:

        case GL_ALPHA:
        case GL_LUMINANCE:
        case GL_LUMINANCE_ALPHA:

        case GL_STENCIL_INDEX:
        case GL_COLOR_INDEX:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL:
            return 1;

        case GL_RG:
        case GL_RG_INTEGER:
            return 2;

        case GL_RGB:
        case GL_BGR:
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return  3;

        case GL_RGBA:
        case GL_BGRA:
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            return 4;
        default:
            return 1;
    }
}

int bytesPerPixel(GLenum format, GLenum type)
{
    switch (type) // several components encoded in type, disregard component count
    {
        case GL_UNSIGNED_BYTE_3_3_2:
        case GL_UNSIGNED_BYTE_2_3_3_REV:
            return 1; // 8 bit

        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_5_6_5_REV:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
            return 2; // 16 bit

        case GL_UNSIGNED_INT_8_8_8_8:
        case GL_UNSIGNED_INT_8_8_8_8_REV:
        case GL_UNSIGNED_INT_10_10_10_2:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_24_8:
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
        case GL_UNSIGNED_INT_5_9_9_9_REV:
        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
            return 4; // 32 bit
    }

    return numberOfComponents(format) * byteSize(type);
}

}

namespace glow {

int imageSizeInBytes(int width, int height, GLenum format, GLenum type)
{
    if (type == GL_BITMAP)
    {
        // handle differently?
        glow::warning() << "imageSizeInBytes: GL_BITMAP not implemented yet";
        return -1;
    }

    int pixelSize = bytesPerPixel(format, type);

    int rowSize = pixelSize * width;

    int alignment = glow::getInteger(GL_PACK_ALIGNMENT); // can be 1, 2, 4 or 8

    rowSize = nextMultiple(rowSize, alignment);

    return rowSize * height;
}

} // namespace glow
