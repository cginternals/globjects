#include "FileRegistry.h"

#include <cassert>

#include <glowutils/File.h>

namespace glowutils
{

FileRegistry* FileRegistry::s_instance = new FileRegistry;

FileRegistry::FileRegistry()
{
}

FileRegistry::~FileRegistry()
{
}

void FileRegistry::registerFile(File * file)
{
    assert(file != nullptr);

    s_instance->m_registeredFiles.insert(file);
}

void FileRegistry::deregisterFile(File * file)
{
    assert(file != nullptr);
    assert(s_instance->m_registeredFiles.find(file) != s_instance->m_registeredFiles.end());

    s_instance->m_registeredFiles.erase(file);
}

void FileRegistry::reloadAll()
{
    for (File* file: s_instance->m_registeredFiles)
    {
        file->reload();
    }
}

} // namespace glowutils
