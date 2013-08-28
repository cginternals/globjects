#pragma once

namespace glow 
{

template<typename T>
class ref_ptr
{
public:
	ref_ptr()
	: m_referenced(nullptr)
	{
	}

	ref_ptr(T * referenced)
	: m_referenced(referenced)
	{
		increaseRef();
	}

	ref_ptr(const ref_ptr & reference)
	{
		m_referenced = reference.m_referenced;
		increaseRef();
	}

	~ref_ptr()
	{
		decreaseRef();
	}

	ref_ptr & operator=(const ref_ptr & reference)
	{
		decreaseRef();
		m_referenced = reference.m_referenced;
		increaseRef();

		return *this;
	}

	T & operator*()
	{
		return *m_referenced;
	}

	const T * operator*() const
	{
		return m_referenced;
	}

	T * operator->()
	{
		return m_referenced;
	}

	const T * operator->() const
	{
		return m_referenced;
	}

	T * get()
	{
		return m_referenced;
	}

	const T * get() const
	{
		return m_referenced;
	}

	operator T *()
	{
		return m_referenced;
	}

	operator const T *() const
	{
		return m_referenced;
	}

	operator bool() const
	{
        return m_referenced ? true : false;
	}

	bool operator<(const ref_ptr & reference) const
	{
		return m_referenced<reference.m_referenced;
	}

	bool operator>(const ref_ptr & reference) const
	{
		return m_referenced>reference.m_referenced;
	}

	bool operator==(const ref_ptr & reference) const
	{
		return m_referenced==reference.m_referenced;
	}

	bool operator!=(const ref_ptr & reference) const
	{
		return m_referenced!=reference.m_referenced;
	}
protected:
	T * m_referenced;

	void increaseRef()
	{
		if (m_referenced) m_referenced->ref();
	}

	void decreaseRef()
	{
		if (m_referenced)
		{
			m_referenced->unref();
		}
	}
};

} // namespace glow
