#pragma once

#include <string>

namespace glow {

class Error;

namespace debugmessageoutput {

void signalError(const Error & error, const std::string & message);

} // namespace debugmessageoutput
} // namespace glow
