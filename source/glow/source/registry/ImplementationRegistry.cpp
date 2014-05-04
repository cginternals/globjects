#include "ImplementationRegistry.h"

#include "Registry.h"

#include "../implementations/AbstractUniformImplementation.h"
#include "../implementations/AbstractBufferImplementation.h"
#include "../implementations/AbstractFrameBufferImplementation.h"

namespace glow {

ImplementationRegistry::ImplementationRegistry()
: m_uniformImplementation(nullptr)
, m_bufferImplementation(nullptr)
, m_frameBufferImplementation(nullptr)
{
}

ImplementationRegistry::~ImplementationRegistry()
{
    delete m_uniformImplementation;
    delete m_bufferImplementation;
    delete m_frameBufferImplementation;
}

ImplementationRegistry & ImplementationRegistry::current()
{
    return Registry::current().implementations();
}

AbstractUniformImplementation & ImplementationRegistry::uniformImplementation()
{
    if (!m_uniformImplementation)
    {
        m_uniformImplementation = AbstractUniformImplementation::create();
    }

    return *m_uniformImplementation;
}

AbstractBufferImplementation & ImplementationRegistry::bufferImplementation()
{
    if (!m_bufferImplementation)
    {
        m_bufferImplementation = AbstractBufferImplementation::create();
    }

    return *m_bufferImplementation;
}

AbstractFrameBufferImplementation & ImplementationRegistry::frameBufferImplementation()
{
    if (!m_frameBufferImplementation)
    {
        m_frameBufferImplementation = AbstractFrameBufferImplementation::create();
    }

    return *m_frameBufferImplementation;
}

} // namespace glow

