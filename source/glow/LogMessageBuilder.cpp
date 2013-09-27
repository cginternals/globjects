#include <glow/LogMessageBuilder.h>

#include <glow/AbstractLogHandler.h>

namespace glow
{

LogMessageBuilder::LogMessageBuilder(LogMessage::Level level, AbstractLogHandler* handler)
:   std::stringstream()
,   m_level(level)
,   m_handler(handler)
{
}

LogMessageBuilder::LogMessageBuilder(const LogMessageBuilder& builder)
: m_level(builder.m_level)
, m_handler(builder.m_handler)
{
	str(builder.str());
}

LogMessageBuilder::~LogMessageBuilder()
{
	if (m_handler)
		m_handler->handle(LogMessage(m_level, str()));
}

LogMessageBuilder& LogMessageBuilder::operator<<(const char * c)
{
	write(c, std::strlen(c));
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const std::string & str)
{
	write(str.c_str(), str.length());
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(bool b)
{
	*this << (b ? "true" : "false");
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(char c)
{
	std::stringstream::operator<<(c);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(int i)
{
	std::stringstream::operator<<(i);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(float f)
{
	std::stringstream::operator<<(f);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(double d)
{
	std::stringstream::operator<<(d);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(long double d)
{
	std::stringstream::operator<<(d);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned u)
{
	std::stringstream::operator<<(u);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(long l)
{
	std::stringstream::operator<<(l);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned long ul)
{
	std::stringstream::operator<<(ul);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned char uc)
{
	std::stringstream::operator<<(uc);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(void * pointer)
{
	std::stringstream::operator<<(pointer);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(std::ostream & (*manipulator)(std::ostream &))
{
	std::stringstream::operator<<(manipulator);
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec2 & v)
{
	std::stringstream::operator<<("vec2(") << v.x << "," << v.y << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec3 & v)
{
	std::stringstream::operator<<("vec3(") << v.x << "," << v.y << "," << v.z << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec4 & v)
{
	std::stringstream::operator<<("vec4(") << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat2 & m)
{
	std::stringstream::operator<<("mat2(")
		<< "(" << m[0][0] << ", " << m[0][1] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat3 & m)
{
	std::stringstream::operator<<("mat3(")
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] <<")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat4 & m)
{
	std::stringstream::operator<<("mat4(")
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] <<"), "
		<< "(" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] <<")"
		<< ")";
	return *this;
}
	
} // namespace glow
