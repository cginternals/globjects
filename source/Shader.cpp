#include <glow/Shader.h>
#include <glow/Program.h>

#include <iostream>
#include <vector>

using namespace glow;

Shader::Shader(GLenum type)
: Object(createShader(type))
, _type(type)
, _sourceFile(nullptr)
, _compiled(false)
{
}

Shader::~Shader()
{
	if (_sourceFile)
	{
		_sourceFile->deregisterShader(this);
	}
	if (_id) glDeleteShader(_id);
}

GLuint Shader::createShader(GLenum type)
{
	return glCreateShader(type);
}

Shader* Shader::fromFile(GLenum type, const std::string& filename)
{
	Shader* shader = new Shader(type);
	ShaderFile* file = new ShaderFile(filename);
	shader->setSourceFile(file);
	return shader;
}

GLenum Shader::type() const
{
	return _type;
}

void Shader::setSource(const std::string& source)
{
	_source = source;
	const char* sourcePointer = source.c_str();
	glShaderSource(_id, 1, &sourcePointer, 0);

	compile();
}

void Shader::setSourceFile(ShaderFile* sourceFile)
{
	_sourceFile = sourceFile;
	_sourceFile->registerShader(this);
	setSource(_sourceFile->content());
}

const std::string& Shader::source() const
{
	return _source;
}

ShaderFile* Shader::sourceFile()
{
	return _sourceFile;
}

void Shader::compile()
{
	glCompileShader(_id);
	checkCompileStatus();

	if (_compiled)
	{
		for (Program* program: _programs)
		{
			program->invalidate();
		}
	}
}

bool Shader::isCompiled() const
{
	return _compiled;
}

std::string Shader::infoLog() const
{
	GLsizei length;
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);

	std::vector<char> log(length);

	glGetShaderInfoLog(_id, length, &length, log.data());

	return std::string(log.data(), length);
}

void Shader::checkCompileStatus()
{
	GLint status = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &status);

	_compiled = (status == GL_TRUE);

	if (!_compiled)
	{
		std::cout
			<< "Compiler error:" << std::endl
			<< "Type " << typeString() << std::endl
			<< infoLog() << std::endl;
	}
}

std::string Shader::typeString()
{
	switch (_type)
	{
		case GL_GEOMETRY_SHADER:
			return "GL_GEOMETRY_SHADER";
		case GL_FRAGMENT_SHADER:
			return "GL_FRAGMENT_SHADER";
		case GL_VERTEX_SHADER:
			return "GL_VERTEX_SHADER";
		case GL_TESS_EVALUATION_SHADER:
			return "GL_TESS_EVALUATION_SHADER";
		case GL_TESS_CONTROL_SHADER:
			return "GL_TESS_CONTROL_SHADER";
		#ifdef GL_COMPUTE_SHADER
		case GL_COMPUTE_SHADER:
			return "GL_COMPUTE_SHADER";
		#endif
		default:
			return "unknown";
	}
}

void Shader::addToProgram(Program* program)
{
	_programs.insert(program);
}

void Shader::removeFromProgram(Program* program)
{
	_programs.erase(program);
}

