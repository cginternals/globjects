#pragma once

namespace glow {

class Error;

namespace debugmessageoutput {

void signalError(const Error & error, const char * file, int line);

} // namespace debugmessageoutput
} // namespace glow
