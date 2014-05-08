#pragma once

#include <chrono>

#include <glbinding/constants.h>



#include <glow/glow_api.h>
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

        Query * query = new Query(gl::TIME_ELAPSED);
        query->begin();
    
        // more GL calls
    
        query->end();
    
        query->wait();
        gl::GLuint timeElapsed = query->get();
        // Note: sometimes it's important to use the 64 bit equivalent, because the 32 bit version can only capture time up to 4 seconds.

    \endcode
    
    An example timestamp access:
    \code{.cpp}

        // GL calls
    
        Query * query = Query::timestamp();
    
        // even more GL calls
    
        query->wait();
        gl::GLuint64 timestamp = query->get64();
        // Note: the result is the timestamp on the GPU right after the first GL calls finished and before the second calls started.

    \endcode
    
    \see http://www.opengl.org/wiki/Query_Object
    \see http://www.opengl.org/registry/specs/ARB/timer_query.txt
 */
class GLOW_API Query : public Object
{
public:
    Query();
    static Query * fromId(gl::GLuint id, bool takeOwnership = false);

    static Query * current(gl::GLenum target);
    static Query * timestamp();
	
    static gl::GLint get(gl::GLenum target, gl::GLenum pname);
    static gl::GLint getIndexed(gl::GLenum target, gl::GLuint index, gl::GLenum pname);

    static gl::GLint getCounterBits(gl::GLenum target);

    virtual void accept(ObjectVisitor& visitor) override;

    void begin(gl::GLenum target) const;
    void end(gl::GLenum target) const;

    void beginIndexed(gl::GLenum target, gl::GLuint index) const;
    void endIndexed(gl::GLenum target, gl::GLuint index) const;
	
    static bool isQuery(gl::GLuint id);

	gl::GLuint get(gl::GLenum pname = gl::QUERY_RESULT) const;
    gl::GLuint64 get64(gl::GLenum pname = gl::QUERY_RESULT) const;
	
	bool resultAvailable() const;
    void wait() const;
    void wait(const std::chrono::duration<int, std::nano> & timeout) const;
	
    gl::GLuint waitAndGet(gl::GLenum pname = gl::QUERY_RESULT) const;
    gl::GLuint64 waitAndGet64(gl::GLenum pname = gl::QUERY_RESULT) const;

    gl::GLuint waitAndGet(const std::chrono::duration<int, std::nano> & timeout, gl::GLenum pname = gl::QUERY_RESULT) const;
    gl::GLuint64 waitAndGet64(const std::chrono::duration<int, std::nano> & timeout, gl::GLenum pname = gl::QUERY_RESULT) const;

    gl::GLuint waitAndGet(gl::GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const;
    gl::GLuint64 waitAndGet64(gl::GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const;
	
    void counter(gl::GLenum target = gl::TIMESTAMP) const;
protected:
	
    Query(gl::GLuint id, bool takeOwnership);
    virtual ~Query();

	static gl::GLuint genQuery();
};

} // namespace glow
