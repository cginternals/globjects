#include <algorithm>
#include <cassert>
#include <unordered_map>

#include <glbinding/constants.h>
#include <glbinding/Extension.h>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/DebugMessage.h>
#include <glow/glow.h>

#include "registry/Registry.h"
#include "registry/ImplementationRegistry.h"
#include "implementations/AbstractDebugImplementation.h"

#include <glow/debugmessageoutput.h>
#include "debugmessageoutput_private.h"

namespace glow {

glow::AbstractDebugImplementation & implementation()
{
    return glow::ImplementationRegistry::current().debugImplementation();
}

namespace debugmessageoutput
{

void setCallback(Callback callback)
{
    implementation().setCallback(callback);
}

void addCallback(Callback callback)
{
    implementation().addCallback(callback);
}

void enable(bool synchronous)
{
    implementation().enable();

    setSynchronous(synchronous);
}

void disable()
{
    implementation().disable();
}

void setSynchronous(bool synchronous)
{
    implementation().setSynchronous(synchronous);
}

void insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, gl::GLsizei length, const char * message)
{
    assert(message != nullptr);

    insertMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

void insertMessage(gl::GLenum source, gl::GLenum type, gl::GLuint id, gl::GLenum severity, const std::string & message)
{
    insertMessage(DebugMessage(source, type, id, severity, message));
}

void insertMessage(const DebugMessage & message)
{
    implementation().insertMessage(message);
}

void enableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, gl::TRUE_);
}

void enableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids)
{
    enableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void disableMessage(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, gl::FALSE_);
}

void disableMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, const std::vector<gl::GLuint> & ids)
{
    disableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void controlMessages(gl::GLenum source, gl::GLenum type, gl::GLenum severity, gl::GLsizei count, const gl::GLuint * ids, gl::GLboolean enabled)
{
    assert(ids != nullptr || count == 0);

    implementation().controlMessages(source, type, severity, count, ids, enabled);
}

void signalError(const Error & error, const std::string & message)
{
    if (!error.isError())
        return;

    if (!Registry::current().isInitialized())
    {
        glow::debug() << "Error during initialization: " << error.name();
        return;
    }

    if (!implementation().isFallback())
        return;

    std::stringstream stream;
    stream << error.name() << " generated: " << message;

    insertMessage(DebugMessage(gl::DEBUG_SOURCE_API_ARB, gl::DEBUG_TYPE_ERROR_ARB, static_cast<unsigned int>(error.code()), gl::DEBUG_SEVERITY_HIGH_ARB, stream.str()));
}

} // namespace debugmessageoutput

} // namespace glow
