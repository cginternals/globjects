
#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <iomanip>

#include <globjects/globjects_api.h>
#include <globjects/base/LogMessageLevel.h>


namespace globjects
{


template <typename T>
class ref_ptr;

class AbstractLogHandler;

/** \brief Builds a LogMessage from different kinds of primitive types.

	The LogMessageBuilder is  usually created by one of the global functions 
    log, debug, warning, error or fatal. It works similar to streams and 
    accepts a number of different types which will be converted to strings 
    automatically. When it goes out of scope, it creates a LogMessage from 
    all streamed objects and sends it to the log handler.

    Typical usage of the LogMessageBuilder:
	\code{.cpp}

		warning() << "This is warning number " << 3;
	
    \endcode

	\see logging.h
	\see LogMessage
	\see setLoggingHandler
    \see setVerbosityLevel
    \see info
	\see debug
	\see warning
    \see critical
*/
class GLOBJECTS_API LogMessageBuilder
{
public:
    // These types are unspecified by the C++ standard -> we need to query the compiler specific types
    using PrecisionManipulator = decltype(std::setprecision(0));
    using FillManipulator = decltype(std::setfill('0'));
    using WidthManipulator = decltype(std::setw(0));

public:
    LogMessageBuilder(LogMessageLevel level, AbstractLogHandler * handler);
    LogMessageBuilder(const LogMessageBuilder & builder);
	virtual ~LogMessageBuilder();

    LogMessageBuilder & operator<<(const char * c);
    LogMessageBuilder & operator<<(const std::string & str);
    LogMessageBuilder & operator<<(bool b);
    LogMessageBuilder & operator<<(char c);
    LogMessageBuilder & operator<<(int i);
    LogMessageBuilder & operator<<(float f);
    LogMessageBuilder & operator<<(double d);
    LogMessageBuilder & operator<<(long double d);
    LogMessageBuilder & operator<<(unsigned u);
    LogMessageBuilder & operator<<(long l);
    LogMessageBuilder & operator<<(long long l);
    LogMessageBuilder & operator<<(unsigned long ul);
    LogMessageBuilder & operator<<(unsigned char uc);
    LogMessageBuilder & operator<<(const void * pointer);

	// manipulators
    LogMessageBuilder & operator<<(std::ostream & (*manipulator)(std::ostream&));
    LogMessageBuilder & operator<<(PrecisionManipulator manipulator);
    LogMessageBuilder & operator<<(FillManipulator manipulator);
#ifndef _MSC_VER
    // in Windows PrecisionManipulator = WidthManipulator
    LogMessageBuilder & operator<<(WidthManipulator manipulator);
#endif
	
    // globjects base objects
    template <typename T>
    LogMessageBuilder & operator<<(const ref_ptr<T> & ref_pointer);
	
	// pointers
	template <typename T>
    LogMessageBuilder & operator<<(const T * pointer);

    // array types
    template <typename T>
    LogMessageBuilder & operator<<(const std::vector<T> & vector);
    template <typename T, std::size_t Count>
    LogMessageBuilder & operator<<(const std::array<T, Count> & array);

protected:
    LogMessageLevel m_level;
    AbstractLogHandler * m_handler;
    std::shared_ptr<std::stringstream> m_stream;
};


} // namespace globjects


#include <globjects/base/LogMessageBuilder.inl>
