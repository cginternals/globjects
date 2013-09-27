#pragma once

#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include <glow/glow.h>
#include <glow/LogMessage.h>

namespace glow {

class AbstractLogHandler;

/** \brief The LogMessageBuilder class builds a LogMessage from different kinds of primitive types.

	The LogMessageBuilder is  usually created by one of the global functions log, debug, warning, error or fatal.
	It works similar to streams and accepts a number of different types which will be converted to strings automatically.
	When it goes out of scope, it creates a LogMessage from all streamed objects and sends it to the log handler.

	Typeical usage of the LogMessageBuilder:
	\code{.cpp}
		warning() << "This is warning number " << 3;
	\endcode

	\see logging.h
	\see LogMessage
	\see setLoggingHandler
	\see log
	\see debug
	\see warning
*/
class GLOW_API LogMessageBuilder : public std::stringstream
{
public:
	LogMessageBuilder(LogMessage::Level level, AbstractLogHandler* handler);
	LogMessageBuilder(const LogMessageBuilder& builder);
	virtual ~LogMessageBuilder();

	// primitive types
	LogMessageBuilder& operator<<(const char* c);
	LogMessageBuilder& operator<<(const std::string& str);
	LogMessageBuilder& operator<<(bool b);
	LogMessageBuilder& operator<<(char c);
	LogMessageBuilder& operator<<(int i);
	LogMessageBuilder& operator<<(float f);
	LogMessageBuilder& operator<<(double d);
	LogMessageBuilder& operator<<(long double d);
	LogMessageBuilder& operator<<(unsigned u);
	LogMessageBuilder& operator<<(long l);
	LogMessageBuilder& operator<<(unsigned long ul);
	LogMessageBuilder& operator<<(unsigned char uc);
	LogMessageBuilder& operator<<(void* pointer);

	// manipulators
	LogMessageBuilder& operator<<(std::ostream& (*manipulator)(std::ostream&));

	// pointers
	template <typename T>
	LogMessageBuilder& operator<<(T* t_pointer);

	// glm types
	LogMessageBuilder& operator<<(const glm::vec2& v);
	LogMessageBuilder& operator<<(const glm::vec3& v);
	LogMessageBuilder& operator<<(const glm::vec4& v);

	LogMessageBuilder& operator<<(const glm::mat2& m);
	LogMessageBuilder& operator<<(const glm::mat3& m);
	LogMessageBuilder& operator<<(const glm::mat4& m);
protected:
	LogMessage::Level m_level;
	AbstractLogHandler* m_handler;
};

} // namespace glow
