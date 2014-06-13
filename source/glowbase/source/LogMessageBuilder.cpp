#include <glowbase/LogMessageBuilder.h>

#include <cstring>
#include <cassert>

#include <glowbase/AbstractLogHandler.h>
#include <glowbase/Version.h>

namespace glow
{

LogMessageBuilder::LogMessageBuilder(LogMessage::Level level, AbstractLogHandler * handler)
: m_level(level)
, m_handler(handler)
, m_stream(new std::stringstream)
{
    assert(handler != nullptr);
}

LogMessageBuilder::LogMessageBuilder(const LogMessageBuilder & builder)
: m_level(builder.m_level)
, m_handler(builder.m_handler)
, m_stream(builder.m_stream)
{
}

LogMessageBuilder::~LogMessageBuilder()
{
    if (m_stream.use_count() > 1)
        return;

	if (m_handler)
        m_handler->handle(LogMessage(m_level, m_stream->str()));
}

LogMessageBuilder & LogMessageBuilder::operator<<(const char * c)
{
    assert(c != nullptr);

    m_stream->write(c, std::strlen(c));
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(const std::string & str)
{
    m_stream->write(str.c_str(), str.length());
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(bool b)
{
	*this << (b ? "true" : "false");
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(char c)
{
    *m_stream << c;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(int i)
{
    *m_stream << i;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(float f)
{
    *m_stream << f;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(double d)
{
    *m_stream << d;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(long double d)
{
    *m_stream << d;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(unsigned u)
{
    *m_stream << u;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(long l)
{
    *m_stream << l;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(long long l)
{
    *m_stream << l;
    return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(unsigned long ul)
{
    *m_stream << ul;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(unsigned char uc)
{
    *m_stream << uc;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(const void * pointer)
{
    *m_stream << pointer;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(std::ostream & (*manipulator)(std::ostream &))
{
    *m_stream << manipulator;
	return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(LogMessageBuilder::PrecisionManipulator manipulator)
{
    *m_stream << manipulator;
    return *this;
}

LogMessageBuilder & LogMessageBuilder::operator<<(LogMessageBuilder::FillManipulator manipulator)
{
    *m_stream << manipulator;
    return *this;
}

#ifndef _MSC_VER
LogMessageBuilder & LogMessageBuilder::operator<<(LogMessageBuilder::WidthManipulator manipulator)
{
    *m_stream << manipulator;
    return *this;
}
#endif

LogMessageBuilder & LogMessageBuilder::operator<<(const Version & version)
{
    *this << "Version " << version.toString();

    return *this;
}

} // namespace glowbase
