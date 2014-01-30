#pragma once

#include <vector>
#include <initializer_list>

#include <glm/glm.hpp>

#include <glow/glow.h>
#include <glow/AbstractArray.h>

namespace glow
{

/** \brief Array datastructure designed for easy use with glow.
    
    It is derived from a std::vector, so the complete interface is available.
    It implements the interface AbstractArray and can reply its internal 
    pointer (rawData()) and the byte size (rawSize()).

    \code{.cpp}

    	Buffer * buffer = new Buffer(GL_ARRAY_BUFFER);

    	Array<glm::vec4> vertices;
    	vertices << glm::vec4(0.0, 0.0, 0.0, 1.0);
    	vertices << glm::vec4(1.0, 0.0, 0.0, 1.0);
    	vertices << glm::vec4(1.0, 1.0, 0.0, 1.0);
    	vertices << glm::vec4(0.0, 1.0, 0.0, 1.0);
    	
    	buffer->setData(vertices);

    \endcode
    
    \see http://www.cplusplus.com/reference/vector/vector/
 */
template<typename T>
class Array : public AbstractArray, public std::vector<T>
{
public:
	Array();

    Array(size_t count);
    Array(size_t count, const T& value);

	Array(const Array<T> & array);

    Array(const std::vector<T> & vector);
    Array(const std::initializer_list<T> & list);

    Array(T * data, unsigned size);
    Array(void * data, unsigned size);

    Array(Array&& other) NOEXCEPT;
	
    virtual const void * rawData() const override;
    virtual GLint rawSize() const override;

	Array & operator<<(const T & element);
};

using FloatArray = Array<float>;
using IntArray = Array<int>;
using UIntArray = Array<unsigned int>;
using ByteArray = Array<char>;
using UByteArray = Array<unsigned char>;

using Vec2Array = Array<glm::vec2>;
using Vec3Array = Array<glm::vec3>;
using Vec4Array = Array<glm::vec4>;

using Mat3Array = Array<glm::mat3>;
using Mat4Array = Array<glm::mat4>;

} // namespace glow

#include <glow/Array.hpp>
