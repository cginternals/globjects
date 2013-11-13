#pragma once

#include <string>

#include <glow/glow.h>
#include <glow/Referenced.h>
#include <glow/Changeable.h>

namespace glow
{

/** \brief Superclass for all types of static and dynamic strings, e.g. for the use as Shader code.
 *
 * The current string can be queried using string().
 *
 * \see Shader
 */
class GLOW_API StringSource : public Referenced, public Changeable
{
public:
    virtual const std::string & string() const = 0;

    virtual std::string shortInfo() const;
};

} // namespace glow
