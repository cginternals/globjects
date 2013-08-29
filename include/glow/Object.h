#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

#include <string>

namespace glow
{

class ObjectVisitor;

class GLOW_API Object : public Referenced
{
public:
	Object();
	Object(GLuint id, bool ownsGLObject = true);
	virtual ~Object();

	virtual void accept(ObjectVisitor& visitor) = 0;

	GLuint id() const;
	operator GLuint() const;

	bool ownsGLObject() const;

	const std::string& name() const;
	void setName(const std::string& name);

protected:
	GLuint m_id;
	bool m_ownsGLObject;
	std::string m_name;
private:
	Object(const Object&);
	Object& operator=(const Object&);

	void registerObject();
	void deregisterObject();
};

} // namespace glow
