#include <globjects/base/LogMessage.h>

namespace globjects
{

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

} // namespace globjects
