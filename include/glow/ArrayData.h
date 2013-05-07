#pragma once

#include <vector>

namespace glow {

class ArrayData
{
public:
	virtual void* rawData() = 0;
	virtual GLenum type() = 0;
	virtual GLint elementSize() = 0;
	virtual GLint byteSize() = 0;
};

template <typename T, GLenum Type, GLint ElementSize = 1>
class Array : public ArrayData, public std::vector<T>
{
public:
	virtual void* rawData() {return (void*)std::vector<T>::data();}
	virtual GLenum type() {return Type;}
	virtual GLint elementSize() {return ElementSize;}
	virtual GLint byteSize() { return std::vector<T>::size()*sizeof(T);}

	Array& operator<<(const T& element) {std::vector<T>::push_back(element); return *this;}
};

} // namespace glow
