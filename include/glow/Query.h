#pragma once

#include <glow/glow.h>

#include <glow/Object.h>

namespace glow
{

/** \brief The Query class encapsulates a GL Query object
 *
 * \see http://www.opengl.org/wiki/Query_Object
 * \see http://www.opengl.org/registry/specs/ARB/timer_query.txt
 */
class GLOW_API Query : public Object
{
public:
	Query();
	Query(GLenum target);
	static Query* timestamp();
	static Query* current(GLenum target);
	
	virtual ~Query();
	
	static int counterBits(GLenum target);

	virtual void accept(ObjectVisitor& visitor);

	void begin();
	void begin(GLenum target);
	void end();
	
	bool isQuery() const;
	
	GLuint get(GLenum pname = GL_QUERY_RESULT) const;
	GLuint64 get64(GLenum pname = GL_QUERY_RESULT) const;
	
	bool resultAvailable() const;
	void wait() const;
	
	void counter(GLenum target = GL_TIMESTAMP);
protected:
	GLenum m_target;
	
	Query(GLuint id, GLenum target);

	static GLuint genQuery();
};

} // namespace glow
