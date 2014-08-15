#include "ImplementationRegistry.h"

#include "Registry.h"

#include "../implementations/AbstractUniformImplementation.h"
#include "../implementations/AbstractBufferImplementation.h"
#include "../implementations/AbstractFrameBufferImplementation.h"
#include "../implementations/AbstractDebugImplementation.h"
#include "../implementations/AbstractProgramBinaryImplementation.h"
#include "../implementations/AbstractShadingLanguageIncludeImplementation.h"

namespace glo {

ImplementationRegistry::ImplementationRegistry()
: m_uniformImplementation(nullptr)
, m_bufferImplementation(nullptr)
, m_frameBufferImplementation(nullptr)
, m_debugImplementation(nullptr)
, m_programBinaryImplementation(nullptr)
, m_shadingLanguageIncludeImplementation(nullptr)
{
    initialize();
}

ImplementationRegistry::~ImplementationRegistry()
{
    delete m_uniformImplementation;
    delete m_bufferImplementation;
    delete m_frameBufferImplementation;
    delete m_debugImplementation;
    delete m_programBinaryImplementation;
    delete m_shadingLanguageIncludeImplementation;
}

ImplementationRegistry & ImplementationRegistry::current()
{
    return Registry::current().implementations();
}

void ImplementationRegistry::initialize()
{
    m_uniformImplementation = AbstractUniformImplementation::create();
    m_bufferImplementation = AbstractBufferImplementation::create();
    m_frameBufferImplementation = AbstractFrameBufferImplementation::create();
    m_debugImplementation = AbstractDebugImplementation::create();
    m_programBinaryImplementation = AbstractProgramBinaryImplementation::create();
    m_shadingLanguageIncludeImplementation = AbstractShadingLanguageIncludeImplementation::create();
}

AbstractUniformImplementation & ImplementationRegistry::uniformImplementation()
{
    return *m_uniformImplementation;
}

AbstractBufferImplementation & ImplementationRegistry::bufferImplementation()
{
    return *m_bufferImplementation;
}

AbstractFrameBufferImplementation & ImplementationRegistry::frameBufferImplementation()
{
    return *m_frameBufferImplementation;
}

AbstractDebugImplementation & ImplementationRegistry::debugImplementation()
{
    return *m_debugImplementation;
}

AbstractProgramBinaryImplementation & ImplementationRegistry::programBinaryImplementation()
{
    return *m_programBinaryImplementation;
}

AbstractShadingLanguageIncludeImplementation & ImplementationRegistry::shadingLanguageIncludeImplementation()
{
    return *m_shadingLanguageIncludeImplementation;
}

} // namespace glo

