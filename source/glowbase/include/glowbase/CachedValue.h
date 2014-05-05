#pragma once

namespace glow
{

/** \brief Extends a value of arbitrary type by an invalidation flag.

    The cached value is intended for use in lazzy initialization, getter or setters
    and removes the overhead of adding those dirty, valid, or update flags to your
    class. Especially when having multiple values, cached value allows per value
    validity flags. Note: all setters are of this class are const, simplifying const
    getters in classes by hiding the value as mutable here.

    Typeical usage of the CachedValue:
    \code{.cpp}

        // on update
        lazzyValue.invalidate();
        ...
        // example getter
        if (!lazzyValue.isValid())
            lazzyValue.setValue(complexCalculationForLazzyValue());

        return lazzyValue.value();

    \endcode
*/
template <typename T>
class CachedValue
{
public:
	CachedValue();
	CachedValue(const T & value);

	bool isValid() const;
	
	T & value();
	const T & value() const;
	void setValue(const T & value, bool validate = true) const;

	void validate() const;
	void invalidate() const;

protected:
    mutable bool m_valid;
	mutable T m_value;
};

} // namespace glowbase

#include <glowbase/CachedValue.hpp>
