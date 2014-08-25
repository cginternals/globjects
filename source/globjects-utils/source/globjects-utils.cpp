#include <globjects-utils/globjects-utils.h>

#include <globjects-base/File.h>

#include <globjects/logging.h>
#include <globjects/NamedString.h>

#ifdef _MSC_VER
#include "windows.h"
#include "dirent_msvc.h"
#else
#include <dirent.h>
#endif

namespace
{

void getFiles(const std::string & dirName, bool recursive, std::vector<std::string> & files)
{
    DIR* dir = opendir(dirName.c_str());
    dirent* entry;
    if (!dir)
    {
        glo::warning() << "Could not open directory " << dirName << ".";
    }
    else
    {
        while ((entry = readdir(dir)))
        {
            std::string name = entry->d_name;

            if (entry->d_type == DT_DIR)
            {
                if (name == "." || name == "..")
                {
                    continue;
                }

                if (recursive)
                {
                    getFiles(dirName + "/" + name, true, files);
                }
            }
            else if (entry->d_type == DT_REG)
            {
                files.push_back(dirName + "/" + entry->d_name);
            }
        }
        closedir(dir);
    }
}

std::vector<std::string> getFiles(const std::string & dirName, bool recursive)
{
    std::vector<std::string> files;

    getFiles(dirName, recursive, files);

    return files;
}

std::string getExtension(const std::string & filename)
{
    size_t pos = filename.find_last_of('.');

    if (pos == std::string::npos)
        return std::string();

    return filename.substr(pos+1);
}

}

using namespace glo;

namespace gloutils
{

void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recursive)
{
    for (const std::string & file: getFiles(directory, recursive))
    {
        std::string extension = getExtension(file);

        if (fileExtension != "*" && extension != fileExtension)
            continue;

        NamedString::create("/"+file, new File(file));
    }
}

} // namespace gloutils
