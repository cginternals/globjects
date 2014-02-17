#include <glowutils/UniformGroup.h>

#include <cassert>

#include <glow/AbstractUniform.h>
#include <glow/Program.h>

#include <glow/logging.h>

using namespace glow;

namespace glowutils 
{

UniformGroup::UniformGroup()
{
}

UniformGroup::~UniformGroup()
{
}

void UniformGroup::addUniform(AbstractUniform * uniform)
{
    assert(uniform != nullptr);

    const std::string name = uniform->name();

    if (m_uniforms.count(name) && m_uniforms.at(name).get() != uniform)
        warning() << "Uniform with name " << name << " already exists on UniformGroup, overwrite it.";

    m_uniforms[name] = uniform;
}

void UniformGroup::addToProgram(Program * program)
{
    assert(program != nullptr);

    for (std::pair<std::string, ref_ptr<AbstractUniform>> pair : m_uniforms)
        program->addUniform(pair.second);
}

} // namespace glowutils
