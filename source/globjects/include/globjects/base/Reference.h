#pragma once

namespace globjects
{

template<typename T>
class Reference
{
public:
	Reference(T * reference);
	~Reference();
	
	T * reference() const;
	void setReference(T * reference);
protected:
    T * m_reference;
};

} // namespace globjects

#include <globjects/base/Reference.hpp>
