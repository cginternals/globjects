#include <glow/Query.h>

#include <glow/ObjectVisitor.h>
#include <glow/Error.h>

namespace glow
{

Query::Query()
: Object(genQuery())
{
}

Query::Query(GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Query * Query::fromId(GLuint id, bool takeOwnership)
{
    return new Query(id, takeOwnership);
}

Query::~Query()
{
	if (ownsGLObject())
	{
		glDeleteQueries(1, &m_id);
		CheckGLError();
	}
}

Query * Query::current(GLenum target)
{
    GLint id = get(target, GL_CURRENT_QUERY);

    if (id <= 0)
    {
        return nullptr;
    }
	
    // TODO: fetch correct query from object registry
    return Query::fromId(id, false);
}

Query * Query::timestamp()
{
    Query * query = new Query();
    query->counter(GL_TIMESTAMP);

    return query;
}

int Query::getCounterBits(GLenum target)
{	
    return get(target, GL_QUERY_COUNTER_BITS);
}

GLuint Query::genQuery()
{
	GLuint id;

	glGenQueries(1, &id);
	CheckGLError();

	return id;
}

GLint Query::get(GLenum target, GLenum pname)
{
    GLint value = 0;

    glGetQueryiv(target, pname, &value);
    CheckGLError();

    return value;
}

GLint Query::getIndexed(GLenum target, GLuint index, GLenum pname)
{
    GLint value = 0;

    glGetQueryIndexediv(target, index, pname, &value);
    CheckGLError();

    return value;
}

void Query::accept(ObjectVisitor& visitor)
{
	visitor.visitQuery(this);
}

void Query::begin(GLenum target) const
{
    glBeginQuery(target, m_id);
    CheckGLError();
}

void Query::end(GLenum target) const
{
    glEndQuery(target);
	CheckGLError();
}

void Query::beginIndexed(GLenum target, GLuint index) const
{
    glBeginQueryIndexed(target, index, m_id);
    CheckGLError();
}

void Query::endIndexed(GLenum target, GLuint index) const
{
    glEndQueryIndexed(target, index);
    CheckGLError();
}

GLuint Query::get(GLenum pname) const
{
    GLuint value = 0;
	
    glGetQueryObjectuiv(m_id, pname, &value);
    CheckGLError();
	
	return value;
}

GLuint64 Query::get64(GLenum pname) const
{
    GLuint64 value = 0;
	
    glGetQueryObjectui64v(m_id, pname, &value);
    CheckGLError();
	
	return value;
}

bool Query::resultAvailable() const
{
	return get(GL_QUERY_RESULT_AVAILABLE) == GL_TRUE;
}

void Query::wait() const
{
    while (!resultAvailable());
}

void Query::wait(const std::chrono::duration<int, std::nano> & timeout) const
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::chrono::high_resolution_clock::time_point current;
    while (!resultAvailable() && start + timeout > current)
    {
        current = std::chrono::high_resolution_clock::now();
    }
}

GLuint Query::waitAndGet(GLenum pname) const
{
    wait();
	
	return get(pname);
}

GLuint64 Query::waitAndGet64(GLenum pname) const
{
    wait();
	
    return get64(pname);
}

GLuint Query::waitAndGet(const std::chrono::duration<int, std::nano> & timeout, GLenum pname) const
{
    return waitAndGet(pname, timeout);
}

GLuint64 Query::waitAndGet64(const std::chrono::duration<int, std::nano> & timeout, GLenum pname) const
{
    return waitAndGet64(pname, timeout);
}

GLuint Query::waitAndGet(GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const
{
    wait(timeout);

    return get(pname);
}

GLuint64 Query::waitAndGet64(GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const
{
    wait(timeout);

    return get64(pname);
}

void Query::counter(GLenum target) const
{
    glQueryCounter(m_id, target);
	CheckGLError();
}

bool Query::isQuery(GLuint id)
{
    bool result = glIsQuery(id) == GL_TRUE;
	CheckGLError();

	return result;
}

} // namespace glow
