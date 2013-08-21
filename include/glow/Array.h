#pragma once

#include <vector>
#include <initializer_list>

#include <glm/glm.hpp>

#include <glow/AbstractArray.h>

namespace glow
{

template<typename T>
class Array : public AbstractArray, public std::vector<T>
{
public:
	Array();
	Array(const Array<T> & array);

	Array(const std::vector<T> & vector);
    Array(const std::initializer_list<T> & list);

    Array(
        T * data
    ,   const unsigned size);
	Array(
        void * data
    ,   const unsigned size);

	virtual const void * rawData() const;
	virtual GLint rawSize() const;

	Array & operator<<(const T & element);
};

typedef Array<float>            FloatArray;
typedef Array<int>              IntArray;
typedef Array<unsigned int>     UIntArray;
typedef Array<char>             ByteArray;
typedef Array<unsigned char>    UByteArray;

typedef Array<glm::vec2>        Vec2Array;
typedef Array<glm::vec3>        Vec3Array;
typedef Array<glm::vec4>        Vec4Array;

} // namespace glow

#include <glow/Array.hpp>
