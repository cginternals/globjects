#pragma once

#include <glow/declspec.h>

#include <glow/Referenced.h>
#include <glow/Changeable.h>

#include <string>

namespace glow {

class GLOW_API ShaderSource : public Referenced, public Changeable
{
public:
	virtual const std::string& source() = 0;
};

} // namespace glow
