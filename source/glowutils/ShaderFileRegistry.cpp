#include <cassert>

#include <glowutils/ShaderFileRegistry.h>

#include <glowutils/ShaderFile.h>

namespace glow
{

ShaderFileRegistry ShaderFileRegistry::s_registry;

ShaderFileRegistry::ShaderFileRegistry()
{
}

ShaderFileRegistry::~ShaderFileRegistry()
{
}

ShaderFileRegistry& ShaderFileRegistry::instance()
{
    return s_registry;
}

bool ShaderFileRegistry::contains(const std::string& filename) const
{
    return m_registeredFiles.count(filename) > 0;
}

ShaderFile * ShaderFileRegistry::obtain(const std::string& filename)
{
    if (!contains(filename))
    {
        return new ShaderFile(filename);
    }

    return m_registeredFiles[filename];
}

void ShaderFileRegistry::registerFile(ShaderFile * file)
{
    assert(file != nullptr);

    m_registeredFiles[file->filePath()] = file;
}

void ShaderFileRegistry::deregisterFile(ShaderFile * file)
{
    assert(file != nullptr);

    m_registeredFiles.erase(file->filePath());
}

void ShaderFileRegistry::reloadAll()
{
    for (const std::pair<std::string, ShaderFile*> pair: m_registeredFiles)
    {
        pair.second->reload();
    }
}

} // namespace glow
