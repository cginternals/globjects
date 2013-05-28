#pragma once

#include <glow/declspec.h>

#include <glow/Object.h>
#include <glow/ShaderFile.h>
#include <glow/ShaderSource.h>

#include <string>
#include <set>

// http://www.opengl.org/wiki/Shader

namespace glow {

class Program;

class GLOW_API Shader : public Object
{
	friend class Program;
	friend class ShaderSource;
public:
	Shader(GLenum type);
	~Shader();

	static Shader* fromFile(GLenum type, const std::string& filename);

	GLenum type() const;

	void setSource(ShaderSource* source);
	void setSource(const std::string& source);

	void compile();
	bool isCompiled() const;

	std::string infoLog() const;
protected:
	void sourceChanged();
	void basicSetSource(const std::string& source);
	bool checkCompileStatus();
	std::string typeString();
protected:
	GLenum _type;
	bool _compiled;
	ref_ptr<ShaderSource> _source;
	std::string _internalSource;
	std::set<Program*> _programs;

	void addToProgram(Program* program);
	void removeFromProgram(Program* program);

	static GLuint createShader(GLenum type);
};

} // namespace glow
