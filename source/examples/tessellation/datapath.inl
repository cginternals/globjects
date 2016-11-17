
#include <string>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>


namespace common
{

    std::string normalizePath(const std::string & filepath)
    {
        auto copy = filepath;
        std::replace(copy.begin(), copy.end(), '\\', '/');

        auto i = copy.find_last_of('/');
        if (i == copy.size() - 1)
            copy = copy.substr(0, copy.size() - 1);

        return copy;
    }

    std::string retrieveDataPath(const std::string & module, const std::string & key)
    {
        const auto moduleInfo = cpplocate::findModule(module);

        auto dataPath = moduleInfo.value(key);
        dataPath = normalizePath(dataPath);

        if (dataPath.empty())
            dataPath = "data/";
        else
            dataPath += "/";

        return dataPath;
    }

}