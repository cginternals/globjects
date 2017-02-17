
#pragma once


#include <set>
#include <string>

#include <globjects/globjects_api.h>


namespace globjects
{


class File;


class GLOBJECTS_API FileRegistry
{
public:
    FileRegistry();
    virtual ~FileRegistry();

    File * get(const std::string & filePath) const;

    void registerFile(File * file);
    void deregisterFile(File * file);

    void reloadAll();


protected:
    std::set<File*> m_registeredFiles;
};


} // namespace globjects
