#pragma once

#include <string>

#include <glow/glow.h>

#include <glow/Referenced.h>
#include <glow/Changeable.h>


namespace glow 
{

class GLOW_API ShaderSource : public Referenced, public Changeable
{
public:
	virtual const std::string & source() = 0;

	virtual bool isFile() const;
};

} // namespace glow
