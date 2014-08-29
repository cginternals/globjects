#include "ImplementationRegistry.h"

#include "Registry.h"

#include "../implementations/AbstractUniformImplementation.h"
#include "../implementations/AbstractBufferImplementation.h"
#include "../implementations/AbstractFrameBufferImplementation.h"
#include "../implementations/AbstractDebugImplementation.h"
#include "../implementations/AbstractProgramBinaryImplementation.h"
#include "../implementations/AbstractShadingLanguageIncludeImplementation.h"
#include "../implementations/AbstractObjectNameImplementation.h"

namespace glo {

ImplementationRegistry::ImplementationRegistry()
: m_uniformImplementation(nullptr)
, m_bufferImplementation(nullptr)
, m_frameBufferImplementation(nullptr)
, m_debugImplementation(nullptr)
, m_programBinaryImplementation(nullptr)
, m_shadingLanguageIncludeImplementation(nullptr)
, m_objectNameImplementation(nullptr)
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
    delete m_objectNameImplementation;
}

ImplementationRegistry & ImplementationRegistry::current()
{
    return Registry::current().implementations();
}

void ImplementationRegistry::initialize() // ToDo: allow for enum parameters with variadic templates for specialization getters..
{
    m_bufferImplementation      = AbstractBufferImplementation::get();
    m_frameBufferImplementation = AbstractFrameBufferImplementation::get();
    m_uniformImplementation     = AbstractUniformImplementation::get();
    m_objectNameImplementation = AbstractObjectNameImplementation::get();

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

AbstractObjectNameImplementation & ImplementationRegistry::objectNameImplementation()
{
    return *m_objectNameImplementation;
}

} // namespace glo
