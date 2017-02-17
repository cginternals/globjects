
#include <globjects/base/FileRegistry.h>

#include <cassert>
#include <algorithm>

#include <globjects/base/File.h>


namespace globjects
{


FileRegistry::FileRegistry()
{
}

FileRegistry::~FileRegistry()
{
}

File * FileRegistry::get(const std::string & filePath) const
{
    const auto it = std::find_if(m_registeredFiles.begin(), m_registeredFiles.end(), [&filePath](const File * file) { return file->filePath() == filePath; });

    if (it == m_registeredFiles.end())
    {
        return nullptr;
    }

    return *it;
}

void FileRegistry::registerFile(File * file)
{
    assert(file != nullptr);

    m_registeredFiles.insert(file);
}

void FileRegistry::deregisterFile(File * file)
{
    assert(file != nullptr);
    assert(m_registeredFiles.find(file) != m_registeredFiles.end());

    m_registeredFiles.erase(file);
}

void FileRegistry::reloadAll()
{
    for (auto file: m_registeredFiles)
    {
        file->reload();
    }
}


} // namespace globjects
