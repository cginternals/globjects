#include <glow/LogMessage.h>

#include <glow/logging.h>


namespace glow {

LogMessage::LogMessage(Level level, const std::string& message)
: _level(level)
, _message(message)
{
}

LogMessage::Level LogMessage::level() const
{
	return _level;
}

const std::string& LogMessage::message() const
{
	return _message;
}

// Builder

LogMessageBuilder::LogMessageBuilder(LogMessage::Level level)
: _level(level)
{
}

LogMessageBuilder::LogMessageBuilder(const LogMessageBuilder& builder)
: _level(builder._level)
{
	_stream.str(builder._stream.str());
}

LogMessageBuilder::~LogMessageBuilder()
{
	LoggingInterface* logger = loggingHandler();
	if (logger)
	{
		logger->handle(LogMessage(_level, _stream.str()));
	}
}

LogMessageBuilder& LogMessageBuilder::operator<<(const char* c)
{
	_stream << c;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const std::string& str)
{
	_stream << str;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(bool b)
{
	_stream << (b ? "true" : "false");
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(char c)
{
	_stream << c;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(int i)
{
	_stream << i;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(float f)
{
	_stream << f;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(double d)
{
	_stream << d;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned u)
{
	_stream << u;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(long l)
{
	_stream << l;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned long ul)
{
	_stream << ul;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(unsigned char uc)
{
	_stream << uc;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(void* pointer)
{
	_stream << pointer;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(std::ostream& (*manipulator)(std::ostream&))
{
	_stream << manipulator;
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec2& v)
{
	_stream << "vec2(" << v.x << "," << v.y << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec3& v)
{
	_stream << "vec3(" << v.x << "," << v.y << "," << v.z << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::vec4& v)
{
	_stream << "vec4(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat2& m)
{
	_stream << "mat2("
		<< "(" << m[0][0] << ", " << m[0][1] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat3& m)
{
	_stream << "mat3("
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] <<")"
		<< ")";
	return *this;
}

LogMessageBuilder& LogMessageBuilder::operator<<(const glm::mat4& m)
{
	_stream << "mat4("
		<< "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "), "
		<< "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] <<"), "
		<< "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] <<"), "
		<< "(" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] <<")"
		<< ")";
	return *this;
}

} // namespace glow
