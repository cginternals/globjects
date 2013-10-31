

#include <glow/AbstractUniform.h>
#include <glow/Program.h>

#include <glow/logging.h>

#include <glowutils/UniformGroup.h>

namespace glow 
{

UniformGroup::UniformGroup()
{
}

UniformGroup::~UniformGroup()
{
}

void UniformGroup::addUniform(AbstractUniform * uniform)
{
    const std::string name = uniform->name();

    if (m_uniforms.count(name) && m_uniforms.at(name).get() != uniform)
        warning() << "Uniform with name " << name << " already exists on UniformGroup, overwrite it.";

    m_uniforms[name] = uniform;
}

void UniformGroup::addToProgram(Program * program)
{
    for (std::pair<std::string, ref_ptr<AbstractUniform>> pair : m_uniforms)
        program->addUniform(pair.second);
}

} // namespace glow
