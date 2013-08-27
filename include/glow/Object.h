#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

#include <glow/ObjectProperties.h>

namespace glow
{

class GLOW_API Object : public Referenced
{
public:
	Object();
	Object(GLuint id, bool ownsGLObject = true);
	virtual ~Object();

	virtual const char* typeName() const = 0;

	GLuint id() const;
	operator GLuint() const;

	bool ownsGLObject() const;

	const std::string& name() const;
	void setName(const std::string& name);

	const ObjectProperties& properties();
protected:
	GLuint m_id;
	bool m_ownsGLObject;
	std::string m_name;
	IF_DEBUG(ObjectProperties m_properties;)
private:
	Object(const Object&);
	Object& operator=(const Object&);

	void registerObject();
	void deregisterObject();
};

} // namespace glow
