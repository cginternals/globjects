#pragma once

#include <glow/declspec.h>

#include <glow/Referenced.h>

#include <string>
#include <set>

namespace glow {

class Shader;

class GLOW_API ShaderSource : public Referenced
{
	friend class Shader;
public:
	virtual const std::string& source() = 0;
protected:
	std::set<Shader*> _shaders;

	void addTo(Shader* shader);
	void removeFrom(Shader* shader);

	void updateShaders();
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
