#include <glow/Query.h>

#include <glbinding/functions.h>

#include <glow/ObjectVisitor.h>
#include <glow/Error.h>

namespace glow
{

Query::Query()
: Object(genQuery())
{
}

Query::Query(gl::GLuint id, bool takeOwnership)
: Object(id, takeOwnership)
{
}

Query * Query::fromId(gl::GLuint id, bool takeOwnership)
{
    return new Query(id, takeOwnership);
}

Query::~Query()
{
	if (ownsGLObject())
	{
        gl::DeleteQueries(1, &m_id);
	}
}

Query * Query::current(gl::GLenum target)
{
    gl::GLint id = get(target, gl::CURRENT_QUERY);

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
    query->counter(gl::TIMESTAMP);

    return query;
}

int Query::getCounterBits(gl::GLenum target)
{	
    return get(target, gl::QUERY_COUNTER_BITS);
}

gl::GLuint Query::genQuery()
{
	gl::GLuint id;

    gl::GenQueries(1, &id);

	return id;
}

gl::GLint Query::get(gl::GLenum target, gl::GLenum pname)
{
    gl::GLint value = 0;

    gl::GetQueryiv(target, pname, &value);

    return value;
}

gl::GLint Query::getIndexed(gl::GLenum target, gl::GLuint index, gl::GLenum pname)
{
    gl::GLint value = 0;

    gl::GetQueryIndexediv(target, index, pname, &value);

    return value;
}

void Query::accept(ObjectVisitor& visitor)
{
	visitor.visitQuery(this);
}

void Query::begin(gl::GLenum target) const
{
    gl::BeginQuery(target, m_id);
}

void Query::end(gl::GLenum target) const
{
    gl::EndQuery(target);
}

void Query::beginIndexed(gl::GLenum target, gl::GLuint index) const
{
    gl::BeginQueryIndexed(target, index, m_id);
}

void Query::endIndexed(gl::GLenum target, gl::GLuint index) const
{
    gl::EndQueryIndexed(target, index);
}

gl::GLuint Query::get(gl::GLenum pname) const
{
    gl::GLuint value = 0;
	
    gl::GetQueryObjectuiv(m_id, pname, &value);
	
	return value;
}

gl::GLuint64 Query::get64(gl::GLenum pname) const
{
    gl::GLuint64 value = 0;
	
    gl::GetQueryObjectui64v(m_id, pname, &value);
	
	return value;
}

bool Query::resultAvailable() const
{
	return get(gl::QUERY_RESULT_AVAILABLE) == gl::TRUE_;
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

gl::GLuint Query::waitAndGet(gl::GLenum pname) const
{
    wait();
	
	return get(pname);
}

gl::GLuint64 Query::waitAndGet64(gl::GLenum pname) const
{
    wait();
	
    return get64(pname);
}

gl::GLuint Query::waitAndGet(const std::chrono::duration<int, std::nano> & timeout, gl::GLenum pname) const
{
    return waitAndGet(pname, timeout);
}

gl::GLuint64 Query::waitAndGet64(const std::chrono::duration<int, std::nano> & timeout, gl::GLenum pname) const
{
    return waitAndGet64(pname, timeout);
}

gl::GLuint Query::waitAndGet(gl::GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const
{
    wait(timeout);

    return get(pname);
}

gl::GLuint64 Query::waitAndGet64(gl::GLenum pname, const std::chrono::duration<int, std::nano> & timeout) const
{
    wait(timeout);

    return get64(pname);
}

void Query::counter(gl::GLenum target) const
{
    gl::QueryCounter(m_id, target);
}

bool Query::isQuery(gl::GLuint id)
{
    return gl::IsQuery(id) == gl::TRUE_;
}

} // namespace glow
