#include <algorithm>
#include <cassert>
#include <unordered_map>

#include <glow/logging.h>
#include <glow/Error.h>
#include <glow/DebugMessage.h>
#include <glow/glow.h>
#include <glow/Extension.h>

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

void insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char * message)
{
    assert(message != nullptr);

    insertMessage(DebugMessage(source, type, id, severity, std::string(message, length)));
}

void insertMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string & message)
{
    insertMessage(DebugMessage(source, type, id, severity, message));
}

void insertMessage(const DebugMessage & message)
{
    implementation().insertMessage(message);
}

void enableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    enableMessages(source, type, severity, 1, &id);
}

void enableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, GL_TRUE);
}

void enableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint> & ids)
{
    enableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void disableMessage(GLenum source, GLenum type, GLenum severity, GLuint id)
{
    disableMessages(source, type, severity, 1, &id);
}

void disableMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids)
{
    controlMessages(source, type, severity, count, ids, GL_FALSE);
}

void disableMessages(GLenum source, GLenum type, GLenum severity, const std::vector<GLuint> & ids)
{
    disableMessages(source, type, severity, static_cast<int>(ids.size()), ids.data());
}

void controlMessages(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
    assert(ids != nullptr || count == 0);

    implementation().controlMessages(source, type, severity, count, ids, enabled);
}

void signalError(const Error & error, const char * file, int line)
{
    if (!error.isError())
        return;

    std::stringstream stream;
    stream << error.name() << " generated. [" << file << ":" << line << "]";

    if (!Registry::current().isInitialized())
    {
        glow::debug() << "Error during initialization: " << stream.str();
        return;
    }

    if (!implementation().isFallback())
        return;

    insertMessage(DebugMessage(GL_DEBUG_SOURCE_API_ARB, GL_DEBUG_TYPE_ERROR_ARB, error.code(), GL_DEBUG_SEVERITY_HIGH_ARB, stream.str()));
}

} // namespace debugmessageoutput

} // namespace glow
