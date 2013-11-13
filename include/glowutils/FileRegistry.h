#pragma once

#include <unordered_map>
#include <string>

#include <glowutils/glowutils.h>

namespace glow
{

class File;

class GLOWUTILS_API FileRegistry
{
public:
    static FileRegistry& instance();

    virtual ~FileRegistry();

    bool contains(const std::string& filename) const;
    File * obtain(const std::string& filename);

    void registerFile(File * file);
    void deregisterFile(File * file);

    void reloadAll();
protected:
    FileRegistry();

    std::unordered_map<std::string, File*> m_registeredFiles;
    static FileRegistry s_registry;
};

} // namespace glow
