#pragma once

#include <string>

#include <GL/glew.h>

#include <glow/glow_api.h>
#include <glow/Referenced.h>


namespace glow
{

class ObjectVisitor;

/** \brief Superclass of all wrapped OpenGL objects.
    
    The superclass is Referenced so that each wrapped OpenGL object supports reference counting.
    Subclasses should call the Object constructor passing a valid OpenGL object name (id) and a flag whether this OpenGL object should be destroyed during the destructor.
    The OpenGL name (id) that was provided in the constructor can be queried using id().
    Additionally, an Object can have meaningful name wich can be get and set using name() and setName().
 */
class GLOW_API Object : public Referenced
{
public:
    virtual void accept(ObjectVisitor & visitor) = 0;

    GLuint id() const;

	bool ownsGLObject() const;

    void takeOwnership();
    void releaseOwnership();

	const std::string & name() const;
	void setName(const std::string & name);

protected:
    Object(GLuint id, bool takeOwnership = true);
    virtual ~Object();

protected:
	GLuint m_id;
	bool m_ownsGLObject;

    std::string m_name;
};

} // namespace glow
