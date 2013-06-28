#pragma once

#include <glow/ArrayData.h>

#include <vector>
#include <glm/glm.hpp>

namespace glow {

template <typename T>
class Array : public ArrayData, public std::vector<T>
{
public:
	Array() {}
	Array(const Array<T>& array) : std::vector<T>(array) {}
	Array(const std::vector<T>& vector) : std::vector<T>(vector) {}
	Array(T* data, unsigned size) : std::vector<T>(data, data+size) {}
	Array(void* data, unsigned size) : std::vector<T>((T*)data, (T*)data+size/sizeof(T)) {}

	virtual const void* rawData() const {return (void*)std::vector<T>::data();}
	virtual GLint byteSize() const { return std::vector<T>::size()*sizeof(T);}

	Array& operator<<(const T& element) {std::vector<T>::push_back(element); return *this;}
};

typedef Array<float> FloatArray;
typedef Array<int> IntArray;
typedef Array<unsigned int> UIntArray;
typedef Array<char> ByteArray;
typedef Array<unsigned char> UByteArray;

typedef Array<glm::vec2> Vec2Array;
typedef Array<glm::vec3> Vec3Array;
typedef Array<glm::vec4> Vec4Array;

} // namespace glow
