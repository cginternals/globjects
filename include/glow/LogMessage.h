#pragma once

#include <glow/glow.h>

#include <sstream>

#include <glm/gtc/type_ptr.hpp>

namespace glow {

class GLOW_API LogMessage
{
public:
	enum Level
	{
		Debug,
		Warning,
		Error,
		Fatal
	};

	LogMessage(Level level, const std::string& message);

	Level level() const;
	const std::string& message() const;
protected:
	Level _level;
	std::string _message;
};

class GLOW_API LogMessageBuilder
{
public:
	LogMessageBuilder(LogMessage::Level level);
	LogMessageBuilder(const LogMessageBuilder& builder);
	~LogMessageBuilder();

	// primitive types
	LogMessageBuilder& operator<<(const char* c);
	LogMessageBuilder& operator<<(const std::string& str);
	LogMessageBuilder& operator<<(bool b);
	LogMessageBuilder& operator<<(char c);
	LogMessageBuilder& operator<<(int i);
	LogMessageBuilder& operator<<(float f);
	LogMessageBuilder& operator<<(double d);
	LogMessageBuilder& operator<<(unsigned u);
	LogMessageBuilder& operator<<(long l);
	LogMessageBuilder& operator<<(unsigned long ul);
	LogMessageBuilder& operator<<(unsigned char uc);
	LogMessageBuilder& operator<<(void* pointer);

	// manipulators
	LogMessageBuilder& operator<<(std::ostream& (*manipulator)(std::ostream&));

	// pointers
	template <typename T>
	LogMessageBuilder& operator<<(T* t_pointer) {return *this << static_cast<void*>(t_pointer); }

	// glm types
	LogMessageBuilder& operator<<(const glm::vec2& v);
	LogMessageBuilder& operator<<(const glm::vec3& v);
	LogMessageBuilder& operator<<(const glm::vec4& v);

	LogMessageBuilder& operator<<(const glm::mat2& m);
	LogMessageBuilder& operator<<(const glm::mat3& m);
	LogMessageBuilder& operator<<(const glm::mat4& m);
protected:
	LogMessage::Level _level;
	std::stringstream _stream;
};

} // namespace glow
