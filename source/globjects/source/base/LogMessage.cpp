#include <globjects/base/LogMessage.h>

namespace globjects
{

LogMessage::LogMessage(const LogMessageLevel level, const std::string & message)
: m_level(level)
, m_message(message)
{
}

LogMessageLevel LogMessage::level() const
{
	return m_level;
}

const std::string & LogMessage::message() const
{
	return m_message;
}

} // namespace globjects
