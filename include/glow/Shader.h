#pragma once

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/ShaderFile.h>
#include <glow/ShaderSource.h>
#include <glow/ChangeListener.h>

#include <string>
#include <set>

// http://www.opengl.org/wiki/Shader

namespace glow {

class Program;

class GLOW_API Shader : public Object, protected ChangeListener, public Changeable
{
	friend class Program;
public:
	Shader(GLenum type);
	Shader(GLenum type, ShaderSource* source);
	virtual ~Shader();

	static Shader* fromFile(GLenum type, const std::string& filename);
	static Shader* fromSource(GLenum type, const std::string& source);

	GLenum type() const;

	void setSource(ShaderSource* source);
	void setSource(const std::string& source);

	void compile();
	bool isCompiled() const;

	std::string infoLog() const;

protected:
	void notifyChanged();
	void updateSource();
	void basicSetSource(const std::string& source);
	bool checkCompileStatus();
	std::string typeString();
protected:
	GLenum _type;
	bool _compiled;
	ref_ptr<ShaderSource> _source;
	std::string _internalSource;

	static GLuint createShader(GLenum type);
};

} // namespace glow
