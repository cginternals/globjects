#include <cassert>

#include <glowutils/FileRegistry.h>

#include <glowutils/File.h>

namespace glowutils
{

FileRegistry FileRegistry::s_registry;

FileRegistry::FileRegistry()
{
}

FileRegistry::~FileRegistry()
{
    for (const std::pair<std::string, File*> pair: m_registeredFiles)
    {
        pair.second->m_registry = nullptr;
    }
}

FileRegistry& FileRegistry::instance()
{
    return s_registry;
}

bool FileRegistry::contains(const std::string& filename) const
{
    return m_registeredFiles.count(filename) > 0;
}

File * FileRegistry::obtain(const std::string& filename)
{
    if (!contains(filename))
    {
        return new File(filename);
    }

    return m_registeredFiles[filename];
}

void FileRegistry::registerFile(File * file)
{
    assert(file != nullptr);

    m_registeredFiles[file->filePath()] = file;
    file->m_registry = this;
}

void FileRegistry::deregisterFile(File * file)
{
    assert(file != nullptr && file->m_registry == this);

    m_registeredFiles.erase(file->filePath());
    file->m_registry = nullptr;
}

void FileRegistry::reloadAll()
{
    for (const std::pair<std::string, File*> pair: m_registeredFiles)
    {
        pair.second->reload();
    }
}

} // namespace glowutils
