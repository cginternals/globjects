#include <glow/LogMessage.h>

#include <glow/logging.h>


namespace glow {

LogMessage::LogMessage(Level level, const std::string& message)
: m_level(level)
, m_message(message)
{
}

LogMessage::Level LogMessage::level() const
{
	return m_level;
}

const std::string& LogMessage::message() const
{
	return m_message;
}

// Builder

LogMessageBuilder::LogMessageBuilder(LogMessage::Level level, LoggingInterface* handler)
: m_level(level)
, m_handler(handler)
{
}

LogMessageBuilder::LogMessageBuilder(const LogMessageBuilder& builder)
: m_level(builder.m_level)
, m_handler(builder.m_handler)
{
	m_stream.str(builder.m_stream.str());
}

LogMessageBuilder::~LogMessageBuilder()
{
	if (m_handler)
	{
		m_handler->handle(LogMessage(m_level, m_stream.str()));
	}
}

LogMessageBuilder& LogMessageBuilder::operator<<(const char* c)
{
	m_stream << c;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const std::string& str)
{
	m_stream << str;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(bool b)
{
	m_stream << (b ? "true" : "false");
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(char c)
{
	m_stream << c;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(int i)
{
	m_stream << i;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(float f)
{
	m_stream << f;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(double d)
{
	m_stream << d;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned u)
{
	m_stream << u;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(long l)
{
	m_stream << l;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned long ul)
{
	m_stream << ul;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned char uc)
{
	m_stream << uc;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(void* pointer)
{
	m_stream << pointer;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(std::ostream& (*manipulator)(std::ostream&))
{
	m_stream << manipulator;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec2& v)
{
	m_stream << "vec2(" << v.x << "," << v.y << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec3& v)
{
	m_stream << "vec3(" << v.x << "," << v.y << "," << v.z << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec4& v)
{
	m_stream << "vec4(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat2& m)
{
	m_stream << "mat2("
		<< "(" << m[0][0] << ", " << m[0][1] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat3& m)
{
	m_stream << "mat3("
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] <<")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat4& m)
{
	m_stream << "mat4("
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] <<"), "
		<< "(" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] <<")"
		<< ")";
	return *this;
}

} // namespace glow
