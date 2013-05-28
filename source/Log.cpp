#include <glow/Log.h>

using namespace glow;

Log::Log()
: device(nullptr)
{
	toStdOut();
}

Log::Device::Device(std::ostream* out, bool free)
: _out(out)
, free(free)
{
}

Log::Device::~Device()
{
	if (free)
	{
		delete _out;
	}
}

std::ostream& Log::Device::out()
{
	return *_out;
}

void Log::directToStdOut()
{
	log.toStdOut();
}

void Log::directToFile(const std::string& filename)
{
	log.toFile(filename);
}

void Log::toStdOut()
{
	delete device;
	device = new Device(&std::cout, false);
}

void Log::toFile(const std::string& filename)
{
	std::ofstream* ofs = new std::ofstream(filename, std::ofstream::out);

	delete device;
	device = new Device(ofs, true);
}

void Log::write(Level level, std::stringstream& ss)
{
	std::string line;
	std::string prefix = levelString(level);

	while (std::getline(ss, line, '\n'))
	{
		device->out() << prefix << line << std::endl;
	}
}

std::string Log::levelString(Level level)
{
	switch (level)
	{
		case Warning:
			return "#warning: ";
		case Error:
			return "#error: ";
		case Fatal:
			return "#fatal: ";
		default:
			return "";
	}
}

Log Log::log;


LogMessage glow::log(Log::Level level)
{
	return LogMessage(level);
}

LogMessage glow::warning()
{
	return LogMessage(Log::Warning);
}

LogMessage glow::error()
{
	return LogMessage(Log::Error);
}

LogMessage glow::fatal()
{
	return LogMessage(Log::Fatal);
}

LogMessage::LogMessage(Log::Level level)
: _level(level)
{
}

LogMessage::LogMessage(const LogMessage& message)
: _level(message._level)
{
	_stream.str(message._stream.str());
}

LogMessage::~LogMessage()
{
	Log::log.write(_level, _stream);
}

LogMessage& LogMessage::operator<<(const char* c)
{
	_stream << c;
	return *this;
}

LogMessage& LogMessage::operator<<(const std::string& str)
{
	_stream << str;
	return *this;
}

LogMessage& LogMessage::operator<<(char c)
{
	_stream << c;
	return *this;
}

LogMessage& LogMessage::operator<<(int i)
{
	_stream << i;
	return *this;
}

LogMessage& LogMessage::operator<<(float f)
{
	_stream << f;
	return *this;
}

LogMessage& LogMessage::operator<<(double d)
{
	_stream << d;
	return *this;
}

LogMessage& LogMessage::operator<<(unsigned u)
{
	_stream << u;
	return *this;
}

LogMessage& LogMessage::operator<<(long l)
{
	_stream << l;
	return *this;
}

LogMessage& LogMessage::operator<<(unsigned long ul)
{
	_stream << ul;
	return *this;
}

LogMessage& LogMessage::operator<<(unsigned char uc)
{
	_stream << uc;
	return *this;
}

LogMessage& LogMessage::operator<<(void* pointer)
{
	_stream << pointer;
	return *this;
}

LogMessage& LogMessage::operator<<(std::ostream& (*manipulator)(std::ostream&))
{
	_stream << manipulator;
	return *this;
}
