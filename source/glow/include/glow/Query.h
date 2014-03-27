#pragma once

#include <chrono>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/Object.h>

namespace glow
{

/** \brief Encapsulates a GL Query object
    
    A Query object is used to query different aspects of the rendering 
    pipeline, such as passed samples, generated primitives, written transform
    feedback primitives or elapsed time. With begin() and end(), the scope of 
    the measurements could be specified. As a Query object queries the GPU 
    asynchroneously, the result won't be available right after the end() 
    invokation. The resultAvailable() method indicates whether the calling of
    get()/get64() will return the final result. To wait until the result is 
    available, the wait() method can be used. Alternatively there are 
    waitAndGet() and waitAndGet64() to query the result in one invokation.
    
    There is a convenience method to create a Query object which answeres with
    the current timestamp on the GPU named timestamp() (see example below).

    An example time measurement:
    \code{.cpp}

        Query * query = new Query(GL_TIME_ELAPSED);
        query->begin();
    
        // more GL calls
    
        query->end();
    
        query->wait();
        GLuint timeElapsed = query->get();
        // Note: sometimes it's important to use the 64 bit equivalent, because the 32 bit version can only capture time up to 4 seconds.

    \endcode
    
    An example timestamp access:
    \code{.cpp}

        // GL calls
    
        Query * query = Query::timestamp();
    
        // even more GL calls
    
        query->wait();
        GLuint64 timestamp = query->get64();
        // Note: the result is the timestamp on the GPU right after the first GL calls finished and before the second calls started.

    \endcode
    
    \see http://www.opengl.org/wiki/Query_Object
    \see http://www.opengl.org/registry/specs/ARB/timer_query.txt
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

    virtual void accept(ObjectVisitor& visitor) override;

	void begin();
	void begin(GLenum target);
	void end();
	
	bool isQuery() const;
	
    void get(GLenum pname, GLuint * param) const;
    void get64(GLenum pname, GLuint64 * param) const;

	GLuint get(GLenum pname = GL_QUERY_RESULT) const;
    GLuint64 get64(GLenum pname = GL_QUERY_RESULT) const;
	
	bool resultAvailable() const;
    void wait() const;
    void wait(const std::chrono::duration<int, std::nano>& timeout) const;
	
    GLuint waitAndGet(GLenum pname = GL_QUERY_RESULT) const;
    GLuint64 waitAndGet64(GLenum pname = GL_QUERY_RESULT) const;

    GLuint waitAndGet(const std::chrono::duration<int, std::nano>& timeout, GLenum pname = GL_QUERY_RESULT) const;
    GLuint64 waitAndGet64(const std::chrono::duration<int, std::nano>& timeout, GLenum pname = GL_QUERY_RESULT) const;

    GLuint waitAndGet(GLenum pname, const std::chrono::duration<int, std::nano>& timeout) const;
    GLuint64 waitAndGet64(GLenum pname, const std::chrono::duration<int, std::nano>& timeout) const;
	
	void counter(GLenum target = GL_TIMESTAMP);
protected:
	GLenum m_target;
	
	Query(GLuint id, GLenum target);

	static GLuint genQuery();
};

} // namespace glow
