#pragma once

/** \brief Extends a value of arbitrary type by an invalidation flag.

    The cached value is intended for use in lazy initialization, getter or setters
    and removes the overhead of adding those dirty, valid, or update flags to your
    class. Especially when having multiple values, cached value allows per value
    validity flags. Note: all setters are of this class are const, simplifying const
    getters in classes by hiding the value as mutable here.

    Typeical usage of the CachedValue:
    \code{.cpp}

        // on update
        lazyValue.invalidate();
        ...
        // example getter
        if (!lazyValue.isValid())
            lazyValue.setValue(complexCalculationForLazyValue());

        return lazyValue.value();

    \endcode
*/
template <typename T>
class CachedValue
{
public:
	CachedValue();
	CachedValue(const T & value);
    CachedValue(T && value);

	bool isValid() const;
	
	T & value();
	const T & value() const;
    void setValue(const T & value, bool validate = true) const;
    void setValue(T && value, bool validate = true) const;

	void validate() const;
	void invalidate() const;

protected:
    mutable bool m_valid;
	mutable T m_value;
};

#include "CachedValue.hpp"
