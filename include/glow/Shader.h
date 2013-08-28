#pragma once

#include <string>
#include <set>

#include <glow/glow.h>

#include <glow/Object.h>
#include <glow/ShaderFile.h>
#include <glow/ShaderSource.h>
#include <glow/ChangeListener.h>

// http://www.opengl.org/wiki/Shader

namespace glow {

class Program;

class GLOW_API Shader : public Object, protected ChangeListener, public Changeable
{
	friend class Program;

public:
    static Shader * fromFile(
        const GLenum type
    ,   const std::string & filename);
    static Shader * fromString(
        const GLenum type
    ,   const std::string & str);

public:
	Shader(const GLenum type);
	Shader(const GLenum type, ShaderSource * source);

	virtual ~Shader();

	virtual const char * typeName() const;

	GLenum type() const;

	void setSource(ShaderSource * source);
	void setSource(const std::string & source);

	bool compile();
	bool isCompiled() const;

	std::string infoLog() const;

protected:
    bool checkCompileStatus();

    void notifyChanged();
	void updateSource();

    std::string typeString() const;
	std::string shaderString() const;

protected:
    static GLuint create(GLenum type);
    static void setSource(
        const Shader & shader
    ,   const std::string & source);

protected:
	GLenum m_type;
    ref_ptr<ShaderSource> m_source;

    std::string m_currentSource;

    bool m_compiled;

};

} // namespace glow
