#pragma once

#include <set>
#include <string>

#include <glowutils/glowutils.h>

namespace glowutils
{
class File;

class GLOWUTILS_API FileRegistry
{
public:
    static void registerFile(File * file);
    static void deregisterFile(File * file);

    static void reloadAll();
protected:
    FileRegistry();
    virtual ~FileRegistry();

    std::set<File*> m_registeredFiles;
    static FileRegistry* s_instance;
};

} // namespace glowutils
