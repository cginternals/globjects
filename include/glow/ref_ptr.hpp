#pragma once

namespace glow {

template<typename T>
class ref_ptr
{
public:
	ref_ptr()
	: _referenced(nullptr)
	{
	}

	ref_ptr(T* referenced)
	: _referenced(referenced)
	{
		increaseRef();
	}

	ref_ptr(const ref_ptr& reference)
	{
		_referenced = reference._referenced;
		increaseRef();
	}

	~ref_ptr()
	{
		decreaseRef();
	}

	ref_ptr& operator=(const ref_ptr& reference)
	{
		decreaseRef();
		_referenced = reference._referenced;
		increaseRef();

		return *this;
	}

	T* operator*()
	{
		return _referenced;
	}

	const T* operator*() const
	{
		return _referenced;
	}

	T* operator->()
	{
		return _referenced;
	}

	const T* operator->() const
	{
		return _referenced;
	}

	T* get()
	{
		return _referenced;
	}

	const T* get() const
	{
		return _referenced;
	}

	operator T*()
	{
		return _referenced;
	}

	operator const T*() const
	{
		return _referenced;
	}

	operator bool() const
	{
		return _referenced;
	}

	bool operator<(const ref_ptr& reference) const
	{
		return _referenced<reference._referenced;
	}

	bool operator>(const ref_ptr& reference) const
	{
		return _referenced>reference._referenced;
	}

	bool operator==(const ref_ptr& reference) const
	{
		return _referenced==reference._referenced;
	}

	bool operator!=(const ref_ptr& reference) const
	{
		return _referenced!=reference._referenced;
	}
protected:
	T* _referenced;

	void increaseRef()
	{
		if (_referenced) _referenced->ref();
	}

	void decreaseRef()
	{
		if (_referenced)
		{
			_referenced->unref();
			if ( _referenced->refCounter()<=0)
			{
				delete _referenced;
			}
		}
	}
};

} // namespace glow
