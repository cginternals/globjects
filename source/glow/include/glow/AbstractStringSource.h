#pragma once

#include <string>
#include <vector>

#include <glowbase/Referenced.h>
#include <glowbase/Changeable.h>

#include <glow/glow_api.h>

namespace glow
{

class CompositeStringSource;

/** \brief Superclass for all types of static and dynamic strings, e.g. for the use as Shader code.
 *
 * The current string can be queried using string().
 *
 * \see Shader
 */
class GLOW_API AbstractStringSource : public Referenced, public Changeable
{
public:
    virtual std::string string() const = 0;
    virtual std::vector<std::string> strings() const;
    std::vector<const AbstractStringSource*> flatten() const;
    virtual void flattenInto(std::vector<const AbstractStringSource*>& vector) const;

    virtual std::string shortInfo() const;
};

} // namespace glow
