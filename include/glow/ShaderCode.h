#pragma once

#include <string>

#include <glow/glow.h>
#include <glow/ShaderSource.h>

namespace glow {

class GLOW_API ShaderCode : public ShaderSource
{
public:
	ShaderCode(const std::string & source);
	virtual const std::string & source();

protected:
	std::string m_source;
};

} // namespace glow
