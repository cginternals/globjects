#include <glow/Query.h>

#include <glow/ObjectVisitor.h>
#include <glow/Error.h>

namespace glow
{

Query::Query()
: Object(genQuery())
, m_target(GLenum(0))
{
}

Query::Query(GLenum target)
: Object(genQuery())
, m_target(target)
{
}

Query::Query(GLuint id, GLenum target)
: Object(id, false)
, m_target(target)
{
}

Query* Query::timestamp()
{
	Query* query = new Query();
	query->counter();
	
	return query;
}

Query::~Query()
{
	if (ownsGLObject())
	{
		glDeleteQueries(1, &m_id);
		CheckGLError();
	}
}

Query* Query::current(GLenum target)
{
    GLint value = 0;
	
	glGetQueryiv(target, GL_CURRENT_QUERY, &value);
	CheckGLError();
	
	return value > 0 ? new Query(value, target) : nullptr; // TODO: fetch correct query from object registry
}

int Query::counterBits(GLenum target)
{
    GLint value = 0;
	
	glGetQueryiv(target, GL_QUERY_COUNTER_BITS, &value);
	CheckGLError();
	
	return value;
}

GLuint Query::genQuery()
{
	GLuint id;

	glGenQueries(1, &id);
	CheckGLError();

	return id;
}

void Query::accept(ObjectVisitor& visitor)
{
	visitor.visitQuery(this);
}

void Query::begin() const
{
	glBeginQuery(m_target, m_id);
	CheckGLError();
}

void Query::begin(GLenum target) const
{
	m_target = target;
	begin();
}

void Query::end() const
{
	glEndQuery(m_target);
	CheckGLError();
}

void Query::get(GLenum pname, GLuint * param) const
{
    glGetQueryObjectuiv(m_id, pname, param);
    CheckGLError();
}

void Query::get64(GLenum pname, GLuint64 * param) const
{
    glGetQueryObjectui64v(m_id, pname, param);
    CheckGLError();
}

GLuint Query::get(GLenum pname) const
{
    GLuint value = 0;
	
    get(pname, &value);
	
	return value;
}

GLuint64 Query::get64(GLenum pname) const
{
    GLuint64 value = 0;
	
    get64(pname, &value);
	
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

void Query::wait(const std::chrono::duration<int, std::nano>& timeout) const
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

GLuint Query::waitAndGet(const std::chrono::duration<int, std::nano>& timeout, GLenum pname) const
{
    return waitAndGet(pname, timeout);
}

GLuint64 Query::waitAndGet64(const std::chrono::duration<int, std::nano>& timeout, GLenum pname) const
{
    return waitAndGet64(pname, timeout);
}

GLuint Query::waitAndGet(GLenum pname, const std::chrono::duration<int, std::nano>& timeout) const
{
    wait(timeout);

    return get(pname);
}

GLuint64 Query::waitAndGet64(GLenum pname, const std::chrono::duration<int, std::nano>& timeout) const
{
    wait(timeout);

    return get64(pname);
}

void Query::counter(GLenum target) const
{
	m_target = target;
	
	glQueryCounter(m_id, m_target);
	CheckGLError();
}

bool Query::isQuery() const
{
	bool result = GL_TRUE == glIsQuery(m_id);
	CheckGLError();
	return result;
}

} // namespace glow
