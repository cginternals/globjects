#pragma once

#include <glow/Object.h>

namespace glow {

class Shader : public Object
{
public:
	Shader(GLenum type);
	~Shader();

	GLenum type() const;

	void source(const char* source);
	void compile();
protected:
	GLenum _type;

	void checkCompileStatus();
};

} // namespace glow
