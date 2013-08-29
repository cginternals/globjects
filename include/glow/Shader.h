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
    ,   const std::string & sourceString);

public:
	Shader(const GLenum type);
	Shader(const GLenum type, ShaderSource * source);

	virtual ~Shader();

	virtual void accept(ObjectVisitor& visitor);

	GLenum type() const;

	void setSource(ShaderSource * source);
	void setSource(const std::string & source);
	 const ShaderSource* source() const;

	bool compile();
	bool isCompiled() const;

	std::string infoLog() const;

    std::string typeString() const;

protected:
    bool checkCompileStatus();

    void notifyChanged();
	void updateSource();

protected:
    static GLuint create(GLenum type);
    static void setSource(
        const Shader & shader
    ,   const std::string & source);

    std::string shaderString() const;

protected:
	GLenum m_type;
    ref_ptr<ShaderSource> m_source;

    std::string m_currentSource;

    bool m_compiled;

};

} // namespace glow
