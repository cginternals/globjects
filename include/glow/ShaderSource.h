#pragma once

#include <glow/declspec.h>

#include <glow/Referenced.h>
#include <glow/Changeable.h>

#include <string>
#include <set>

namespace glow {


class GLOW_API ShaderSource : public Referenced, public Changeable
{
public:
	virtual const std::string& source() = 0;
};


class GLOW_API ShaderCode : public ShaderSource
{
public:
	ShaderCode(const std::string& code);

	virtual const std::string& source();
protected:
	std::string _code;
};

} // namespace glow
