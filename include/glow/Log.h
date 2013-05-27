#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <glow/declspec.h>


namespace glow {

class GLOW_API Log
{
	friend class LogMessage;
public:
	enum Level
	{
		Debug,
		Warning,
		Error,
		Fatal
	};

	static void directToStdOut();
	static void directToFile(const std::string& filename);
protected:
	class Device
	{
	public:
		Device(std::ostream* out, bool free);
		~Device();

		std::ostream& out();
	protected:
		std::ostream* _out;
		bool free;
	};

	Log();
	Device* device;

	void toStdOut();
	void toFile(const std::string& filename);

	static Log log;
	void write(Level level, std::stringstream& ss);
	std::string levelString(Level level);
};

class GLOW_API LogMessage
{
public:
	LogMessage(Log::Level level = Log::Debug);
	LogMessage(const LogMessage& message);
	~LogMessage();

	LogMessage& operator<<(const char* c);
	LogMessage& operator<<(const std::string& str);
	LogMessage& operator<<(char c);
	LogMessage& operator<<(int i);
	LogMessage& operator<<(void* pointer);

	LogMessage& operator<<(std::ostream& (*manipulator)(std::ostream&));

	template <typename T>
	LogMessage& operator<<(T* t_pointer) {return *this << static_cast<void*>(t_pointer); }
protected:
	Log::Level _level;
	std::stringstream _stream;
};

LogMessage log(Log::Level level = Log::Debug);
LogMessage warning();
LogMessage error();
LogMessage fatal();


} // namespace glow
