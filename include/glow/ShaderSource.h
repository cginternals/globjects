#pragma once

#include <string>

#include <glow/glow.h>

#include <glow/Referenced.h>
#include <glow/Changeable.h>


namespace glow 
{

class Shader;

/** \brief Superclass for all types of sources of a Shader.
 
    The current source can be queried using source().
    
    \see Shader
 */
class GLOW_API ShaderSource : public Referenced, public Changeable
{
public:
    virtual const std::string & source() const = 0;
    virtual std::string shortInfo() const;
    virtual std::set<Shader*> requiredShaders() const;
};

} // namespace glow
