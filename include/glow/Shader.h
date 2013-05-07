#pragma once

#include <glow/Object.h>

#include <string>
#include <set>

namespace glow {

class Program;

class Shader : public Object
{
	friend class Program;
public:
	Shader(GLenum type);
	~Shader();

	GLenum type() const;

	void setSource(const std::string& source, bool compile = true);
	void compile();
	bool isCompiled() const;

	std::string infoLog() const;
protected:
	void checkCompileStatus();
	std::string typeString();
protected:
	GLenum _type;
	bool _compiled;
	std::set<Program*> _programs;
};

} // namespace glow
