
#include <string>

#include <cpplocate/cpplocate.h>

#include <globjects/globjects.h>


namespace common
{

std::string determineDataPath()
{
    std::string path = cpplocate::locatePath("data/qt-example", "share/globjects/qt-example", reinterpret_cast<void *>(&globjects::detachAllObjects));
    if (path.empty()) path = "./data";
    else              path = path + "/data";

    return path;
}

}
