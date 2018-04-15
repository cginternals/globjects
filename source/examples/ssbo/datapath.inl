
#include <string>

#include <cpplocate/cpplocate.h>

#include <globjects/globjects.h>


namespace common
{

std::string determineDataPath()
{
    std::string path = cpplocate::locatePath("data/ssbo", "share/globjects/ssbo", reinterpret_cast<void *>(&globjects::detachAllObjects));
    if (path.empty()) path = "./data";
    else              path = path + "/data";

    return path;
}

}
