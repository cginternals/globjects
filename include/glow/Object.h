#pragma once

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Referenced.h>

#include <string>

namespace glow
{

class ObjectVisitor;

/**
 * \brief The Object class is the superclass of all wrapped OpenGL objects.
 *
 * The superclass is Referenced so that each wrapped OpenGL object supports reference counting.
 * Subclasses should call the Object constructor passing a valid OpenGL object name (id) and a flag whether this OpenGL object should be destroyed during the destructor.
 * The OpenGL name (id) that was provided in the constructor can be queried using id().
 * Additionally, an Object can have meaningful name wich can be get and set using name() and setName().
 */
class GLOW_API Object : public Referenced
{
public:
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

	Object(GLuint id, bool ownsGLObject = true);
private:
	Object();
	Object(const Object&);
	Object& operator=(const Object&);

	void registerObject();
	void deregisterObject();
};

} // namespace glow
