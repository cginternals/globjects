#include <glowutils/glowutils.h>

#include <glow/logging.h>
#include <glow/NamedString.h>
#include <glow/Shader.h>

#include <glowutils/File.h>

#ifdef _MSC_VER
#include "windows.h"
#include "dirent_msvc.h"
#else
#include <dirent.h>
#endif

namespace
{

std::vector<std::string> getFiles(const std::string & dirName)
{
    std::vector<std::string> files;

    DIR* dir = opendir(dirName.c_str());
    dirent* entry;
    if (!dir)
    {
        glow::warning() << "Could not open directory " << dirName << ".";
    }
    else
    {
        while ((entry = readdir(dir)))
        {
            std::string filename(entry->d_name);
            if (filename == "." || std::string(entry->d_name) == "..")
                continue;

            files.push_back(filename);
        }
        closedir(dir);
    }

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

using namespace glow;

namespace glowutils
{

Shader * createShaderFromFile(const GLenum type, const std::string& fileName)
{
    return new Shader(type, new File(fileName));
}

Shader * createShaderFromFile(GLenum type, const std::string & fileName, const std::vector<std::string> & includePaths)
{
    return new Shader(type, new File(fileName), includePaths);
}

void scanDirectory(const std::string & directory, const std::string & fileExtension)
{
    for (const std::string & file: getFiles(directory))
    {
        std::string extension = getExtension(file);

        if (fileExtension != "*" && extension != fileExtension)
            continue;

        std::string fileName = directory+"/"+file;

        NamedString::create("/"+fileName, new File(fileName));
    }
}

} // namespace glowutils
